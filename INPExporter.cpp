#include "fileio.h"
#include "MeshShape/patch.h"
#include "MeshShape/meshshape.h"


bool INPExporter::exportShape(Shape* pShape, const char *fname){

    MeshShape* pMS = dynamic_cast<MeshShape*>(pShape);
    if (!pMS)
        return false;

    Mesh_p mesh = pMS->mesh();

    ofstream outfile;
    outfile.open (fname);

    outfile<<"*Heading"<<endl;
    outfile<<"** Job name: Job-1 Model name: Model-1"<<endl;
    outfile<<"** Generated by: Abaqus/CAE 6.12-1"<<endl;
    outfile<<"** Preprint, echo=NO, model=NO, history=NO, contact=NO"<<endl;
    outfile<<"** "<<endl;
    outfile<<"** PARTS"<<endl;
    outfile<<"** "<<endl;
    outfile<<"** Part, name=Part-1"<<endl;

    FaceList faces = mesh->faces();

    int   facenum = 0;
    int*  nodePage = new int[faces.size()];

    nodePage[0] = 1;
    outfile<<"*Node"<<endl;
    FOR_ALL_ITEMS(FaceList, faces){
        Face_p pF = (*it);

        if (!pF->pData || !pF->pData->pSurface)
            continue;

        GridPattern* patch = dynamic_cast<GridPattern*>(pF->pData->pSurface);
        if (!patch)
            continue;

        int fi = nodePage[facenum];
        int Nu,Nv;
        Nu = Nv = patch->getN();

        for(int j=0; j < Nv; j++){
            for(int i = 0; i < Nu; i++){
                Point p = patch->P(i, j);
                int id = i + j*Nu;
                outfile<<id<<", "<<p.x<<", "<<p.y<<", "<<", 0." <<endl;
            }
        }

        nodePage[facenum+1] = nodePage[facenum] + Nu*Nv;
        facenum++;
    }

    //now quads
    int fi = 0;
    int*  elementPage = new int[faces.size()];
    elementPage[0] = 1;

    std::list<int> elements[5];

    outfile<<endl;
    outfile<<"*Element, type=S4R"<<endl;
    FOR_ALL_ITEMS(FaceList, faces){
        Face_p pF = (*it);

        if (!pF->pData || !pF->pData->pSurface)
            continue;

        GridPattern* patch = dynamic_cast<GridPattern*>(pF->pData->pSurface);
        if (!patch)
            continue;

        int nodeP = nodePage[fi];
        int Nu,Nv;
        Nu = Nv = patch->getN();
        int Ni = patch->getN()-1;

        for(int j=0; j<(Nv-1); j++){
            for(int i=0; i<(Nu-1); i++){
                int id[4] = {patch->ind(i,  j), patch->ind(i+1,  j), patch->ind(i+1,  j+1), patch->ind(i,  j+1)};
                int eid = elementPage[fi] + i + j*(Nu-1);
                outfile<<eid<<", "<<(id[0]+nodeP)<<", "<<(id[1]+nodeP)<<", "<<(id[2]+nodeP)<<", "<<(id[3]+nodeP)<<endl;
                int pat = patch->getPattern(i, j);
                pat = (pat<0)?0:pat;
                elements[pat].push_back(eid);
            }
        }

        elementPage[(fi+1)] = elementPage[fi] + (Nu-1)*(Nv-1);
        fi++;
    }


    //now sets
    fi = 0;
    outfile<<endl;
    for(int i=0; i<5; i++){
        if (elements[i].empty())
            continue;
            outfile<<"*Nset, nset = Set-"<<i<< endl;
        FOR_ALL_ITEMS(std::list<int>, elements[i]){
            int eid = *it;
            if (it == elements[i].begin())
                outfile<<eid;
            else
                outfile<<", "<<eid;
        }
         outfile<<endl;
    }
    outfile.close();
}


/*bool INPExporter::exportShape(Shape* pShape, const char *fname){

    MeshShape* pMS = dynamic_cast<MeshShape*>(pShape);
    if (!pMS)
        return false;

    Mesh_p mesh = pMS->mesh();

    ofstream outfile;
    outfile.open (fname);

    outfile<<"*Heading"<<endl;
    outfile<<"** Job name: Job-1 Model name: Model-1"<<endl;
    outfile<<"** Generated by: Abaqus/CAE 6.12-1"<<endl;
    outfile<<"*Preprint, echo=NO, model=NO, history=NO, contact=NO"<<endl;
    outfile<<"**"<<endl;

    outfile<<"** PARTS"<<endl;
    outfile<<"**"<<endl;

    FaceList faces = mesh->faces();
    int fid = 0;

    FOR_ALL_ITEMS(FaceList, faces){
        Face_p pF = (*it);

        if (!pF->pData || !pF->pData->pSurface)
            continue;

        GridPattern* patch = dynamic_cast<GridPattern*>(pF->pData->pSurface);
        if (!patch)
            continue;

        outfile<<"*Part, name=Part-"<<(fid+1)<<endl;

    }

}

/*
bool INPExporter::exportShape(Shape* pShape, const char *fname){

    MeshShape* pMS = dynamic_cast<MeshShape*>(pShape);
    if (!pMS)
        return false;

    Mesh_p mesh = pMS->mesh();

    ofstream outfile;
    outfile.open (fname);

    outfile<<"*Heading"<<endl;
    outfile<<"** Job name: Job-1 Model name: Model-1"<<endl;
    outfile<<"** Generated by: Abaqus/CAE 6.12-1"<<endl;
    outfile<<"*Preprint, echo=NO, model=NO, history=NO, contact=NO"<<endl;
    outfile<<"**"<<endl;
    outfile<<"** PARTS"<<endl;
    outfile<<"**"<<endl;
    outfile<<"*Part, name=Part-1"<<endl;

    FaceList faces = mesh->faces();

    int   facenum = 0;
    int*  nodePage = new int[faces.size()];

    nodePage[0] = 1;
    outfile<<"*Node"<<endl;
    FOR_ALL_ITEMS(FaceList, faces){
        Face_p pF = (*it);

        if (!pF->pData || !pF->pData->pSurface)
            continue;

        UVPatternPatch* patch = dynamic_cast<UVPatternPatch*>(pF->pData->pSurface);
        if (!patch)
            continue;

        int fi = nodePage[facenum];

        for(int uv = 0; uv<2; uv++){

            for (int n = 0; n < patch->UV(uv); n++){

                for(int j = 0; j<2; j++){
                    for(int i = 0; i < patch->getN(); i++){
                        int id = patch->ind(uv, n, i, j) + fi;
                        Point p = patch->P(uv, n, i, j);
                        outfile<<id<<", "<<p.x<<", "<<p.y<<", "<<", 0." <<endl;
                    }
                }

            }
        }
        nodePage[facenum+1] = nodePage[facenum] + (patch->U() + patch->V())*patch->getN()*2;
        facenum++;
    }


    //now quads
    int fi = 0;

    int*  elementPage = new int[faces.size()];
    elementPage[0] = 1;


    outfile<<endl;
    outfile<<"*Element, type=S4R"<<endl;
    FOR_ALL_ITEMS(FaceList, faces){
        Face_p pF = (*it);

        if (!pF->pData || !pF->pData->pSurface)
            continue;

        UVPatternPatch* patch = dynamic_cast<UVPatternPatch*>(pF->pData->pSurface);
        if (!patch)
            continue;

        int nodeP = nodePage[fi];
        int Ni = patch->getN()-1;

        for(int uv = 0; uv<2; uv++){

            for (int n = 0; n < patch->UV(uv); n++){
                for(int i = 0; i < Ni; i++){
                    int id[4] = {patch->ind(uv, n, i, 0), patch->ind(uv, n, i, 1), patch->ind(uv, n, i+1, 1), patch->ind(uv, n, i+1, 0)};
                    int eid   = elementPage[fi] + i + n*Ni + uv*patch->U()*Ni;
                    outfile<<eid<<", "<<(id[0]+nodeP)<<", "<<(id[1]+nodeP)<<", "<<(id[2]+nodeP)<<", "<<(id[3]+nodeP)<<endl;
                }
            }
        }

        elementPage[(fi+1)] = elementPage[fi] + (patch->U() + patch->V())*Ni;
        fi++;
    }


    //now sets
    fi = 0;
    outfile<<endl;
    int si = 1;
    FOR_ALL_ITEMS(FaceList, faces){
        Face_p pF = (*it);

        if (!pF->pData || !pF->pData->pSurface)
            continue;

        UVPatternPatch* patch = dynamic_cast<UVPatternPatch*>(pF->pData->pSurface);
        if (!patch)
            continue;

        int Ni = patch->getN()-1;
        for(int uv = 0; uv<2; uv++){

            for (int n = 0; n < patch->UV(uv); n++){

                outfile<<"*Nset, nset=Set-"<<(si++)<<endl;
                for(int i = 0; i < Ni; i++){
                    int eid   = elementPage[fi] + i + n*Ni + uv*patch->U()*Ni;
                    if (i<(Ni-1))
                        outfile<<eid<<", ";
                    else
                        outfile<<eid<<endl;
                }
            }
        }
        fi++;
    }

    outfile.close();
}
*/
