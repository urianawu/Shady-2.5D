#include "meshcommands.h"

void onSplitEdge(Corner_p c, double t)
{
    bool isforward = true;
    if (!c->isC0())
    {
        c = c->vNext();
        isforward = false;
    }

    MeshShape* pMS = (MeshShape*) c->V()->mesh()->caller();

    c->F()->Face::update(true);
    if (c->other())
        c->other()->F()->Face::update(true);

    //clean up
    Edge_p e0 = c->prev()->E();
    ShapeVertex_p svtan = e0->pData->pSV[2];
    c->V()->pData->adopt(e0->pData->pSV[2]);
    if (svtan->pair()){
        svtan->pair()->setPair(c->E()->pData->pSV[2]);
    }

    //parenting fixed
    //now adjust control p's
    Bezier* curve0 = e0->pData->pCurve;

    Point newCP[7];
    //bool isforward = curve0->pCV(0) == e0->C0()->V()->pData->pP();
    curve0->calculateDivideCV(isforward? t : (1-t), newCP);

    curve0->set(c->V()->pData->pP(), 3);

    curve0->pCV(1)->set(newCP[1]);
    curve0->pCV(2)->set(newCP[2]);
    curve0->pCV(3)->set(newCP[3]);

    Bezier* curve1 = c->E()->pData->pCurve;

    curve1->pCV(1)->set(newCP[4]);
    curve1->pCV(2)->set(newCP[5]);


}


Point computeVerticalTangent(double t, Edge_p pE, Face_p pF =0)
{
    bool isforward = (!pF) || (pF == pE->C0()->F());

    Point p[8];
    p[0] = pE->pData->pCurve->CV(0);
    p[1] = pE->pData->pCurve->CV(1);
    p[2] = pE->pData->pCurve->CV(2);
    p[3] = pE->pData->pCurve->CV(3);

    if (isforward){

        p[4] = pE->C0()->prev()->E()->pData->getTangentSV(pE->C0())->P;
        p[7] = pE->C0()->next()->E()->pData->getTangentSV(pE->C0()->next())->P;

    }else{
        p[4] = pE->C1()->next()->E()->pData->getTangentSV(pE->C1()->next())->P;
        p[7] = pE->C1()->prev()->E()->pData->getTangentSV(pE->C1())->P;
        t=1-t;
    }

    p[5] = p[4] + p[1] - p[0];
    p[6] = p[7] + p[2] - p[3];

    Point p45 = p[4]*(1-t) + p[5]*(t);
    Point p56 = p[5]*(1-t) + p[6]*(t);
    Point p67 = p[6]*(1-t) + p[7]*(t);

    Point p0 = p45*(1-t) + p56*t;
    Point p1 = p56*(1-t) + p67*t;
    return p0*(1-t) + p1*t;
}

void MeshOperation::insertSegment(Edge_p e, const Point & p){

    if (!e )
        return;

    Mesh_p pMesh = e->mesh();
    MeshShape* pMS = (MeshShape*)pMesh->caller();

    list<Corner_p> clist;

    double t;
    e->pData->pCurve->computeDistance(p, t);

    Point tan0 = computeVerticalTangent(t, e);
    Corner* c0 = pMesh->splitEdge(e, pMS->addMeshVertex());
    onSplitEdge(c0, t);

    Corner* c1 = c0->vNext();
    Face_p endf = (c1)?c1->F():0;

    while(c0 && c0->F()!=endf){

        Point tan1  = computeVerticalTangent((1-t), c0->next()->next()->E(), c0->F());

        Corner_p c0nnvn = c0->next()->next()->vNext();
        Face_p f1 = c0nnvn ? c0nnvn->F():0;
        Point tan00 = computeVerticalTangent(t, c0->next()->next()->E(),f1);

        Corner* c01 = pMesh->splitEdge(c0->next()->next()->E(), pMS->addMeshVertex(), c0->F());
        onSplitEdge(c01, 1-t);

        Corner* c0n = c01->vNext();
        Edge_p pEnew = pMesh->insertEdge(c0, c01);
        pEnew->pData->pSV[1]->P.set(tan0);
        pEnew->pData->pSV[2]->P.set(tan1);
        tan0 = tan00;

        clist.push_back(c0);
        c0 = c0n;
    }

    if (c0 && c0->F() == endf){
        (pMesh->insertEdge(c0, c0->next()->next()->next()));
    }else while(c1){
        Corner* c11 = pMesh->splitEdge(c1->next()->next()->E(), pMS->addMeshVertex(), c1->F());
        onSplitEdge(c11, t);
        Corner* c1n = c11->vNext();
        pMesh->insertEdge(c1, c11);
        clist.push_back(c1);
        c1 = c1n;
    }

}

void MeshOperation::diagonalDivide(Corner_p c){
    if (!c)
        return;
    Mesh_p pMesh = c->V()->mesh();
    Edge_p e = pMesh->insertEdge(c, c->next()->next(), true);
}

Face_p MeshOperation::extrude(Face_p f0, double t){

    Mesh_p pMesh = f0->mesh();
    MeshShape* pMS = (MeshShape*)pMesh->caller();

    f0->update();
    Face_p f1 = pMesh->addFace(f0->size());

    Point pmid;//init as centroid of P

     for(int i=0; i<f0->size(); i++){
         pmid = pmid + P0(f0->C(i));
     }
     pmid = pmid*(1.0/f0->size());

    for(int i=0; i<f0->size(); i++)
    {
        Point p = P0(f0->C(i)) * (1-t) + pmid * t;
        f1->set(pMS->addMeshVertex(p), i);
    }

    Edge_p e3 = 0;
    Face_p f_side_0 = 0;
    for(int i=0; i<f1->size(); i++){
        Face_p f_side =  pMesh->addFace(4);

        //set verts
        f_side->set(f0->C(i)->V(), 0);
        f_side->set(f0->C(i+1)->V(), 1);
        f_side->set(f1->C(i+1)->V(), 2);
        f_side->set(f1->C(i)->V(), 3);
        f_side->Face::update();

        f0->C(i)->E()->set(f_side->C(0), f0->C(i)->isC0()?0:1);

        Edge_p e1 = pMesh->addEdge(f_side->C(1),0);
        Edge_p e2 = pMesh->addEdge(f1->C(i), f_side->C(2));

        if (e3)
            e3->set(f_side->C(3),1);
        else
            f_side_0  = f_side;

        e3 = e1;
    }

    e3->set(f_side_0->C(3),1);
    f1->update();

    pMesh->remove(f0);
    //*f0 = *f_side_0; //replace 1st side face with the old face
    return f1;
}

Edge_p MeshOperation::extrude(Edge_p e0, double t, bool isSmooth, VertexMap *pVMap){

    if (!e0 || !e0->isBorder())
        return 0;

    Mesh_p pMesh = e0->mesh();
    MeshShape* pMS = (MeshShape*)pMesh->caller();

    Face_p f = pMesh->addFace(4);

    //set verts
    Vec2 n = ( Vec3(0,0,1) % Vec3(P1(e0) - P0(e0)) ).normalize()*t;

    Vertex_p v0 = 0;
    Vertex_p v1 = 0;

    Edge_p e1 = 0;
    Edge_p e3 = 0;

    if (pVMap){
        std::map<Vertex_p, Corner_p>::iterator it_v = pVMap->find(e0->C0()->V());
        if (it_v == pVMap->end()){
            v0 = pMS->addMeshVertex(P0(e0)+n);
            (*pVMap)[e0->C0()->V()] = f->C(1);
        }else{
            Corner_p ec = it_v->second;
            e1 = ec->E();
            v0 = (ec->V() == e0->C0()->V() || ec->V() == e0->C0()->next()->V())? ec->next()->V():ec->V();
            v0->pData->P = P0(e0) + ((v0->pData->P + P0(e0)+n)*0.5).normalize()*t; 
        }

        it_v = pVMap->find(e0->C0()->next()->V());
        if (it_v == pVMap->end()){
            v1 = pMS->addMeshVertex(P1(e0)+n);
            (*pVMap)[e0->C0()->next()->V()] = f->C(3);
        }else{
            Corner_p ec = it_v->second;
            e3 = ec->E();
            v1 = (ec->V() == e0->C0()->V() || ec->V() == e0->C0()->next()->V())? ec->next()->V():ec->V();
            v1->pData->P = P1(e0) + ((v1->pData->P + P1(e0)+n)*0.5).normalize()*t;
        }

    }else{
        v0 = pMS->addMeshVertex(P0(e0)+n);
        v1 = pMS->addMeshVertex(P1(e0)+n);
    }

    f->set(e0->C0()->next()->V(), 0);
    f->set(e0->C0()->V(), 1);
    f->set(v0, 2);
    f->set(v1, 3);

    //insert edges
    e0->set(f->C(0));
    if (e1)
        e1->set(f->C(1));
    else
        pMesh->addEdge(f->C(1), 0); //e1

    Edge_p e2 = pMesh->addEdge(f->C(2), 0);

    if (e3)
        e3->set(f->C(3));
    else
        pMesh->addEdge(f->C(3), 0); //e3

    f->Face::update(false, e0->C0()->I()+2);

    if (isSmooth && !pVMap){
        for(int i=0; i<4; i++)
            MeshShape::makeSmoothCorners(f->C(i),true, 1);
    }

    return e2;
}

void MeshOperation::deleteFace(Face_p f, Cache *pCache){

    if (!f)
        return;

    if (pCache)
        pCache->push_back(FaceCache(f));

    MeshShape* pMS = (MeshShape*)(f->mesh()->caller());

    for(int i=0; i<f->size(); i++){
        if (f->C(i)->E()->isBorder() && f->C(i-1)->E()->isBorder()){
            pMS->removeVertex(f->C(i)->V()->pData);
        }
        if (f->C(i)->E()->isBorder()){
            pMS->removeVertex(f->C(i)->E()->pData->pSV[1]);
            pMS->removeVertex(f->C(i)->E()->pData->pSV[2]);
            //delete f->C(i)->E()->pData->pCurve;
        }
    }

    pMS->mesh()->remove(f, true);
    /*if (mesh->sizeF()==0)u
    {
        //Session::get()->removeShape((Shape_p)mesh->caller());
    }*/
}
