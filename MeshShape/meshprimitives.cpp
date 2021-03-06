#include "meshcommands.h"
#include "cmesh.h"

using namespace dlfl;

MeshShape* MeshPrimitive::insertGrid(const Point& p, double nlen, double mlen, int n, int m, MeshShape *pMS){

    if (!pMS)
        pMS = new MeshShape();

    Point p0 = p - Point(nlen*n/2, -mlen*m/2);

    m = m?m:n;
    Vertex_p* vs = new Vertex_p[(n+1)*(m+1)];
    for(int j=0; j<m+1; j++)
        for(int i=0; i<n+1; i++){
            Point pp = Point(nlen*i,0)+ Point(0,-mlen*j)+p0;
            if (pp.norm()>0)
            {
                pp = p + ((MeshShape::isSMOOTH)? (pp.normalize()*Max(fabs(pp.x),fabs(pp.y)) ):pp);
            }
            vs[i+j*(n+1)] = pMS->addMeshVertex(pp);
        }

    for(int j=0; j<m; j++)
        for(int i=0; i<n; i++)
            pMS->_control->addQuad(vs[i+j*(n+1)], vs[i+1+j*(n+1)], vs[i+1+(j+1)*(n+1)], vs[i+(j+1)*(n+1)] );

    delete vs;
    pMS->_control->buildEdges();
    if (MeshShape::isSMOOTH)
        pMS->makeSmoothTangents();
    pMS->Renderable::update();
    return pMS;
}
/*
MeshShape* MeshPrimitive::insertNGon(const Point& p, int n, int segv, double rad, MeshShape *pMS){

    if (!pMS)
        pMS = new MeshShape();

    CLAMP(segv, 1, 4);
    CLAMP(n, 2, 8);


    Face_p pF =  pMS->mesh()->addFace(n);
    for(int i=0; i<n; i++)
    {
        float ang = -2*PI/n*i;
        Vertex_p v = pMS->addMeshVertex(Point(rad*cos(ang), rad*sin(ang)));
        v->set(pF->C(i));
    }

    pMS->_control->buildEdges();
    if (MeshShape::isSMOOTH)
        pMS->makeSmoothTangents();
    pMS->Renderable::update();
    return pMS;

}*/

MeshShape* MeshPrimitive::insertNGon(const Point& p, int n, int segv, double rad, MeshShape *pMS){

    if (!pMS)
        pMS = new MeshShape();

    CLAMP(segv, 1, 4);
    CLAMP(n, 2, 8);

    int nn = (NGON_HANDLING == SUBDIV)? 2*n : n;
    Vertex_p vmid = (NGON_HANDLING == POLYGON) ? 0: pMS->addMeshVertex();
    Vertex_p* vs = new Vertex_p[nn*segv];

    for(int j=0; j<segv; j++){
        for(int i=0; i<nn; i++){
            double ang = -i*PI/n*(NGON_HANDLING == SUBDIV ? 1:2);
            double angv = j*PI/(2*segv);
            vs[i + j*nn] = pMS->addMeshVertex(p + Point(rad*cos(ang), rad*sin(ang))*cos(angv));
        }
    }

    if (segv > 1){
        for(int j=0; j<segv-1; j++){
            for(int i=0; i<nn; i++){
                pMS->_control->addQuad(vs[i + j*nn], vs[(i+1)%nn + j*nn], vs[(i+1)%nn + (j+1)*nn], vs[i + (j+1)*nn] );
            }
        }
    }

    int mid_off = (segv-1)*nn;
    Face_p fmid = (NGON_HANDLING == POLYGON)? pMS->_control->addFace(nn) : 0;
    for(int i=0; i<n; i++){
        if (NGON_HANDLING == SUBDIV){
            Face_p f = pMS->_control->addQuad(vmid, vs[i*2+mid_off], vs[i*2+1+mid_off], vs[(i*2+2)%nn+mid_off]);
            f->reoffset(i%4);
        }else if (NGON_HANDLING == PIE){
            Face_p f = pMS->_control->addTriangle(vmid, vs[i+mid_off], vs[(i+1)%nn+mid_off]);
        }else if (NGON_HANDLING == POLYGON)
            vs[i]->set(fmid->C(i));
    }
    delete vs;

    pMS->_control->buildEdges();
    if (MeshShape::isSMOOTH)
        pMS->makeSmoothTangents();
    pMS->Renderable::update();
    return pMS;

}

MeshShape* MeshPrimitive::insertTorus(const Point& p, int n, int v, double rad_x, double rad_y, double w, double arc, MeshShape* pMS)
{

    double evenW = -1.0;

    evenW = MIN(rad_x,rad_y)*w;

    if (!pMS)
        pMS = new MeshShape();

    bool isarc = arc < 0.9999;
    bool ispie = (w>0.999);

    int segU = n;
    int nV = (v+1);
    int nU = segU + isarc;

    double step_u = 2*PI / segU * arc;
    Vertex_p* vs = new Vertex_p[nU*nV];

    FOR_ALL_J(nV){
        double t = j*1.0/(nV-1);
        FOR_ALL_I(nU){
            double ang_u = -step_u * i;
            Point p(cos(ang_u)*rad_x, sin(ang_u)*rad_y);
            Point pp;
            if (evenW>0)
            {
                pp = p*(1-t) + (p.normalize()*(p.norm() - evenW))*(t);
            }else{
                pp = p*(1.0 - w/(nV-1)*j );
            }
            if (ispie && j == v){
                if (i==0)
                    vs[v*nU] = pMS->addMeshVertex(pp);
            }else
                vs[i+j*nU] = pMS->addMeshVertex(pp);
        }
    }

    FOR_ALL_J(nV-1){
        FOR_ALL_I(segU){

            if (ispie && j == (v-1))
                pMS->_control->addTriangle(vs[v*nU], vs[i+j*nU], vs[(i+1)%nU + j*nU]);
            else
                pMS->_control->addQuad(vs[i+j*nU] , vs[(i+1)%nU + j*nU], vs[(i+1)%nU + (j+1)*nU], vs[i + (j+1)*nU]);
        }
    }

    delete vs;
    pMS->_control->buildEdges();

    if (MeshShape::isSMOOTH){
        double k =  8.0/segU;
        pMS->makeSmoothTangents(false, 1, pow(k,k/3.0));
    }

    pMS->Renderable::update();
    return pMS;
}
