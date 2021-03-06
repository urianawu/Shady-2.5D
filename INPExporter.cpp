#include "fileio.h"
#include "MeshShape/pattern.h"
#include "MeshShape/meshshape.h"
#include <sstream>

#define NODEPAGE 10000
#define EPSILON 0.00001

int INPExporter::mapNodeId(GridPattern* patch, int i, int j, int node_id){
    _nodeMap[patch->face()->id() * NODEPAGE + patch->ind(i,j)] = node_id;
}

int INPExporter::nodeIndex(GridPattern* patch, int i, int j){

    int ci = patch->cornerI(i, j);
    int ei = patch->edgeI(i, j);

    if (ci== -1 && ei == -1)
        return _nodeMap[patch->face()->id()*NODEPAGE + patch->ind(i,j) ];//_nodePage[patch->face()->id()] + patch->ind(i,j);

    if (ci!=-1){
        int vid = patch->face()->C(ci)->V()->id();
        Corner_p pC = (Corner_p) _vertexNodes[vid];

        if (pC->F() == patch->face())
            return  _nodeMap[pC->F()->id()*NODEPAGE + patch->ind(i,j) ];

        //now return the node from the neigbor
        Patch* patch1 = (Patch*)pC->F()->pData;
        return  _nodeMap[pC->F()->id()*NODEPAGE + patch1->edgeInd(pC->I(), 0)];
    }

    int eid = patch->face()->C(ei)->E()->id();

    Corner_p pC = (Corner_p) _edgeNodes[eid];

    if (pC->F() == patch->face())
        return  _nodeMap[pC->F()->id()*NODEPAGE + patch->ind(i,j)];

    //now return the node from the neigbor
    Patch* patch1 = (Patch*)pC->F()->pData;
    return _nodeMap[pC->F()->id()*NODEPAGE + patch1->edgeUInd(pC->I(), (pC->I()%2)?j:i)];
}

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
    outfile<<"* Preprint, echo=NO, model=NO, history=NO, contact=NO"<<endl;
    outfile<<"** "<<endl;
    outfile<<"** PARTS"<<endl;
    outfile<<"** "<<endl;
    outfile<<"* Part, name=Part-1"<<endl;

    mesh->enamurateFaces();
    mesh->enamurateEdges();
    mesh->enamurateVerts();

    FaceList faces = mesh->faces();

    //this is a caching for common nodes
    _edgeNodes = (void**) new void*[mesh->sizeE()];
    FOR_ALL_I(mesh->sizeE())
            _edgeNodes[i] = 0;

    _vertexNodes = (void**) new void*[mesh->size()];
    FOR_ALL_I(mesh->size())
            _vertexNodes[i] = 0;

    list<int> allnodes;

    int node_id = 1;
    outfile<<"*Node"<<endl;
    FOR_ALL_ITEMS(FaceList, faces){
        Face_p pF = (*it);

        if (!pF->pData || pF->isDeleted())
            continue;

        GridPattern* patch = dynamic_cast<GridPattern*>(pF->pData);
        if (!patch)
            continue;

        int Nu = patch->USamples();
        int Nv = patch->VSamples();

        for(int j=0; j < Nv; j++){
            for(int i = 0; i < Nu; i++){


                int ci = patch->cornerI(i, j);
                if (ci != -1){
                    int vid =  pF->C(ci)->V()->id();
                    if (_vertexNodes[vid]){
                       continue;
                    }else{
                        _vertexNodes[vid] = (void*)pF->C(ci);
                    }
                }

                int ei = patch->edgeI(i, j);
                if (ei != -1){
                    int eid =  pF->C(ei)->E()->id();
                    if (_edgeNodes[eid] && _edgeNodes[eid]!= pF->C(ei) ){
                       continue;
                    }else{
                        _edgeNodes[eid] = (void*)pF->C(ei);
                    }
                }

                Point p = patch->P(i, j);
                allnodes.push_back(node_id);
                float x = fabs(p.x) > EPSILON ? (float)p.x : 0;
                float y = fabs(p.y) > EPSILON ? (float)p.y : 0;

                outfile<<node_id<<", "<<x<<", "<<y<<", "<<" 0." <<endl;
                mapNodeId(patch, i, j, node_id);
                node_id++;
            }
        }
    }

   //return true;

    //now quads
    int fi = 0;
    int*  elementPage = new int[faces.size()];
    elementPage[0] = 1;

    std::list<int> elements[3];

    std::set<int> foldnodes[2];

    outfile<<endl;
    outfile<<"*Element, type=S4R"<<endl;
    FOR_ALL_ITEMS(FaceList, faces){
        Face_p pF = (*it);

        if (!pF->pData || pF->isDeleted())
            continue;

        GridPattern* patch = dynamic_cast<GridPattern*>(pF->pData);
        if (!patch)
            continue;

        int Nu = patch->USamples();;
        int Nv = patch->VSamples();

        for(int j=0; j<(Nv-1); j++){
            for(int i=0; i<(Nu-1); i++){
                //int id[4] = {patch->ind(i,  j), patch->ind(i+1,  j), patch->ind(i+1,  j+1), patch->ind(i,  j+1)};

                int id[4] = {nodeIndex(patch, i,  j), nodeIndex(patch, i+1,  j), nodeIndex(patch, i+1,  j+1), nodeIndex(patch, i,  j+1)};
                int eid = elementPage[fi] + i + j*(Nu-1);                
                outfile<<eid<<", "<<id[0]<<", "<<id[1]<<", "<<id[2]<<", "<<id[3]<<endl;

                int pat = patch->getPattern(i, j);

                if (pat>0){
                    for(int k=0; k<4; k++)
                        foldnodes[pat-1].insert(id[k]);
                }

                pat = (pat<0)?0:pat;
                elements[pat].push_back(eid);
            }
        }

        elementPage[(fi+1)] = elementPage[fi] + (Nu-1)*(Nv-1);
        fi++;
    }

    //output all nodes since Edwin asked for it
    int count = 0;
    outfile<<endl;
    outfile<<"*Nset, nset=AllNodes"<<endl;
    FOR_ALL_ITEMS(list<int>, allnodes){
        int id = *it;
        if ((count%16)==0)
            outfile<<id;
        else
            outfile<<", "<<id;
        count++;
        if ((count%16)==0)
            outfile<<endl;
    }

    //output all fold nodes since Edwin asked for it
    outfile<<endl<<endl;

    for(int i =0; i<2; i++){
        count = 0;
        outfile<<"*Nset, nset=FoldNode"<<(i+1)<<endl;
        FOR_ALL_ITEMS(std::set<int>, foldnodes[i]){
            int id = *it;
            if ((count%16)==0)
                outfile<<id;
            else
                outfile<<", "<<id;
            count++;
            if ((count%16)==0)
                outfile<<endl;
        }
        outfile<<endl;
    }

    //now sets
    count = 0;
    fi = 0;
    outfile<<endl;
    outfile<<endl;
    //all sets in both normal directions
    outfile<<"*Elset, elset=Set0"<<endl;
    for(int i=0; i<3; i++)
    {
        FOR_ALL_ITEMS(std::list<int>, elements[i]){
            int eid = *it;
            if ((count%16)==0)
                outfile<<eid;
            else
                outfile<<", "<<eid;
            count++;
            if ((count%16)==0)
                outfile<<endl;
        }
    }
    outfile<<endl;

    outfile<<"*Surface, type=ELEMENT, name = Set0_Bot"<<endl;
    outfile<<"Set0, SNEG"<<endl;

    outfile<<"*Surface, type=ELEMENT, name = Set0_Top"<<endl;
    outfile<<"Set0, SPOS"<<endl;

    outfile<<endl;

    fi = 0;
    outfile<<endl;
    for(int i=1; i<3; i++)
    {
        if (elements[i].empty())
            continue;

        //outfile<<"*Nset, nset = Set-"<<i<< endl;
        std::ostringstream name;
        name<<"ElemSet"<<i;
        outfile<<"*Elset, elset="<<name.str()<<endl;

        count = 0;
        FOR_ALL_ITEMS(std::list<int>, elements[i]){
            int eid = *it;
            if ((count%16)==0)
                outfile<<eid;
            else
                outfile<<", "<<eid;
            count++;
            if ((count%16)==0)
                outfile<<endl;
        }
        outfile<<endl;

        if (i==1)
        {
            outfile<<"*Surface, type=ELEMENT, name="<<name.str()<<"_Bot"<<endl;
            outfile<<name.str()<<", SNEG"<<endl;
        }else if (i==2){
            outfile<<"*Surface, type=ELEMENT, name="<<name.str()<<"_Top"<<endl;
            outfile<<name.str()<<", SPOS"<<endl;
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
