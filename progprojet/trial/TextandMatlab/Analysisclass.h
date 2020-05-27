#include "Systeme.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include "Dalle.h"
#include <vector>
#include "Sphere.h"
#include "Brique.h"
#include "Dodec.h"
#include "Cylinder.h"
#include <iostream>


//A set of three magnets 'meeting' at a vertex
struct vertexmagnets {
    Vecteur3D position;
    int       magnet1; // index
    int       magnet2; // index
    int       magnet3; // index
};

//A set of two Vertex magnets (5 magnets total) sharing an edge and the index of the central magnet in the table of magnets as an int;this central magnet could be more elegantly stored as a pointer

struct doublevertex {
    int           centralmagnet;
    vertexmagnets v1;
    vertexmagnets v2;
};

//A set of five magnets on the 5 edges of a dodecahedron face
struct facemagnets {
    std::vector<doublevertex> doublevertexVector;
    double                    orientation;
};


// A dodecahedron with 30 magents initialised with angles found in the file "finalangles.in" if it exists
class SystemeDodec {
public:
    // Attributes:
    Dodec dode;
    Systeme s;

    //N.B. functions and structures are easily generalisable to all solids, would be better with classes
    //Suggestion: replace indices by pointers

    // Tables (arrays) containing all the Vertexmagnets, Doublevertex and Faces:
    std::vector<vertexmagnets> VM; //Vertexmagnets table
    std::vector<doublevertex> DV; //Doublevertex table
    std::vector<facemagnets> FM; //Faces table

    // Constructor:
    SystemeDodec(std::string finalangles = "finalangles.in") : dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0,
          1.23607), false)
    {

        // VM initialisation:
        std::vector<std::vector<Vecteur3D> > vertexpositions = dode.vertipositions();


        for (size_t i = 0; i < vertexpositions.size(); ++i) {
            size_t si = (vertexpositions[i]).size();
            for (size_t j = 0; j < si; ++j) {
                int exists = -1;
                for (size_t l = 0; l < VM.size(); ++l) {
                    if (vertexpositions[i][j] == VM[l].position) { exists = l; }
                }
                if (exists == -1) {
                    VM.push_back({ vertexpositions[i][j], -1, -1, -1 });
                }
            }
        }

        for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
            size_t si = ((dode.vertipositions())[i]).size();
            for (size_t j = 0; j < si; ++j) {
                //Adding all magnets in magnet table (array) in the system
                Vecteur3D pos( ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2);
                Vecteur3D polaraxis = (dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si];
                Vecteur3D v2        =
                  ((dode.vertipositions())[i][(j + 1) % si] - (dode.vertipositions())[i][(j + 2) % si]);
                Vecteur3D v3  = polaraxis ^ v2;
                double alph1  = -1 * atan(2) / 2;
                Vecteur3D v4  = v3.rotate(alph1, polaraxis);
                Vecteur3D axe = (v4 ^ polaraxis).normalise();

                MagnetA M(pos, axe, 1, 0, polaraxis); // position pos, axis axe, movable yes, angle_0 0, polaraxis

                int index= s.addMagnet(M);

                //VM attributes calculation
                for (auto& vm: VM) {
                    if (((vm).position - (dode.vertipositions())[i][(j) % si]).norme() < 0.01) {
                        if (vm.magnet1 == -1 and vm.magnet1 != index and vm.magnet2 != index and vm.magnet3 != index) {
                            vm.magnet1 = index;
                        }
                        else if (vm.magnet2 == -1 and vm.magnet1 != index and vm.magnet2 != index and vm.magnet3 !=
                          index)
                        {
                            vm.magnet2 = index;
                        }
                        else if (vm.magnet3 == -1 and vm.magnet1 != index and vm.magnet2 != index and vm.magnet3 !=
                          index) { vm.magnet3 = index; }
                    }
                }
            }
        }


        // DV:
        for (size_t i = 0; i < VM.size(); ++i) {
            for (size_t j = i + 1; j < VM.size(); ++j) {
                if (VM[i].magnet1 == VM[j].magnet1 or VM[i].magnet1 == VM[j].magnet2 or VM[i].magnet1 ==
                  VM[j].magnet3)
                {
                    DV.push_back({ VM[i].magnet1, VM[i], VM[j] });
                }

                if (VM[i].magnet2 == VM[j].magnet1 or VM[i].magnet2 == VM[j].magnet2 or VM[i].magnet2 ==
                  VM[j].magnet3)
                {
                    DV.push_back({ VM[i].magnet2, VM[i], VM[j] });
                }

                if (VM[i].magnet3 == VM[j].magnet1 or VM[i].magnet3 == VM[j].magnet2 or VM[i].magnet3 ==
                  VM[j].magnet3)
                {
                    DV.push_back({ VM[i].magnet3, VM[i], VM[j]  });
                }
            }
        }

        // Facevector (FM):
        for (size_t i = 0; i < vertexpositions.size(); ++i) {
            std::vector<doublevertex> VectorDVelem;
            size_t si = (vertexpositions[i]).size();
            for (size_t j = 0; j < si; ++j) {
                for (auto DVelem: DV) {
                    if (DVelem.v1.position == vertexpositions[i][j] and DVelem.v2.position ==
                      vertexpositions [i][(j + 1) % si]) { VectorDVelem.push_back(DVelem); }

                    if (DVelem.v2.position == vertexpositions[i][j] and DVelem.v1.position ==
                      vertexpositions [i][(j + 1) % si]) { VectorDVelem.push_back(DVelem); }
                }
            }
            FM.push_back({ VectorDVelem, -10 });
        }

        // set angles:
        s.setangles(finalangles);
    }


    // Methods:

    // N.B. Vertex strength: (f/w):

    //Method to return the type of the Vertex {SSS,SSN,SNN,NSS,NSN,NNS,NNN}
    std::string OddoneoutText(vertexmagnets vm)
    {
        double N1N2 = (((s.tab_ptr_Magnets[vm.magnet1])->positionN())
          - ((s.tab_ptr_Magnets[vm.magnet2])->positionN())).norme();
        double N1S2 = (((s.tab_ptr_Magnets[vm.magnet1])->positionN())
          - ((s.tab_ptr_Magnets[vm.magnet2])->positionS())).norme();
        double S1N2 = (((s.tab_ptr_Magnets[vm.magnet1])->positionS())
          - ((s.tab_ptr_Magnets[vm.magnet2])->positionN())).norme();
        double S1S2 = (((s.tab_ptr_Magnets[vm.magnet1])->positionS())
          - ((s.tab_ptr_Magnets[vm.magnet2])->positionS())).norme();
        double N2N3 = (((s.tab_ptr_Magnets[vm.magnet2])->positionN())
          - ((s.tab_ptr_Magnets[vm.magnet3])->positionN())).norme();
        double N2S3 = (((s.tab_ptr_Magnets[vm.magnet2])->positionN())
          - ((s.tab_ptr_Magnets[vm.magnet3])->positionS())).norme();
        double S2N3 = (((s.tab_ptr_Magnets[vm.magnet2])->positionS())
          - ((s.tab_ptr_Magnets[vm.magnet3])->positionN())).norme();
        double S2S3 = (((s.tab_ptr_Magnets[vm.magnet2])->positionS())
          - ((s.tab_ptr_Magnets[vm.magnet3])->positionS())).norme();


        // cases: N1S2 (N3/S3):
        if (N1S2 < N1N2 and N1S2 < S1S2 and N1S2 < S1N2) {
            if (S2N3 > S2S3) { return std::to_string(vm.magnet1) + "N1S2S3"; }
            else { return std::to_string(vm.magnet2) + "N1S2N3"; }
        }
        // cases: N1N2 (N3/S3)
        else if (N1N2 < N1S2 and N1N2 < S1S2 and N1N2 < S1N2) {
            if (N2N3 > N2S3) { return std::to_string(vm.magnet3) + "N1N2S3";  }
            else { return std::to_string(-1) + "N1S2N3"; }
        }

        // cases: S1S2 (N3/S3):
        else if (S1S2 < N1S2 and S1S2 < N1N2 and S1S2 < S1N2) {
            if (S2N3 > S2S3) { return std::to_string(-2) + "S1S2S3"; }
            else { return std::to_string(vm.magnet3) + "S1S2N3"; }
        }
        // cases: S1N2 (N3/S3)
        else {
            if (S1N2 < N1S2 and S1N2 < N1N2 and S1N2 < S1S2) { return std::to_string(vm.magnet2) + "S1N2S3"; }
            else { return std::to_string(vm.magnet1) + "S1N2S3"; }
        }

    } // sends back oddmagnet index

    //Method to return the index of the magnet of the odd magnet among the three magnets of a vertexmagnet
    int Oddoneout(vertexmagnets vm)
    {
        //        double N1N2 = (((s.tab_ptr_Magnets[vm.magnet1])->positionN())
        //          - ((s.tab_ptr_Magnets[vm.magnet2])->positionN())).norme();
        //        double N1S2 = (((s.tab_ptr_Magnets[vm.magnet1])->positionN())
        //          - ((s.tab_ptr_Magnets[vm.magnet2])->positionS())).norme();
        //        double S1N2 = (((s.tab_ptr_Magnets[vm.magnet1])->positionS())
        //          - ((s.tab_ptr_Magnets[vm.magnet2])->positionN())).norme();
        //        double S1S2 = (((s.tab_ptr_Magnets[vm.magnet1])->positionS())
        //          - ((s.tab_ptr_Magnets[vm.magnet2])->positionS())).norme();
        //        double N2N3 = (((s.tab_ptr_Magnets[vm.magnet2])->positionN())
        //          - ((s.tab_ptr_Magnets[vm.magnet3])->positionN())).norme();
        //        double N2S3 = (((s.tab_ptr_Magnets[vm.magnet2])->positionN())
        //          - ((s.tab_ptr_Magnets[vm.magnet3])->positionS())).norme();
        //        double S2N3 = (((s.tab_ptr_Magnets[vm.magnet2])->positionS())
        //          - ((s.tab_ptr_Magnets[vm.magnet3])->positionN())).norme();
        //        double S2S3 = (((s.tab_ptr_Magnets[vm.magnet2])->positionS())
        //          - ((s.tab_ptr_Magnets[vm.magnet3])->positionS())).norme();
        double P1N2 = (s.tab_ptr_Magnets[vm.magnet1]->position
          - s.tab_ptr_Magnets[vm.magnet2]->positionN()).norme();
        double P1S2 = (s.tab_ptr_Magnets[vm.magnet1]->position
          - s.tab_ptr_Magnets[vm.magnet2]->positionS()).norme();
        double P2N1 = (s.tab_ptr_Magnets[vm.magnet2]->position
          - s.tab_ptr_Magnets[vm.magnet1]->positionN()).norme();
        double P2S1 = (s.tab_ptr_Magnets[vm.magnet2]->position
          - s.tab_ptr_Magnets[vm.magnet1]->positionS()).norme();
        double P1N3 = (s.tab_ptr_Magnets[vm.magnet1]->position
          - s.tab_ptr_Magnets[vm.magnet3]->positionN()).norme();
        double P1S3 = (s.tab_ptr_Magnets[vm.magnet1]->position
          - s.tab_ptr_Magnets[vm.magnet3]->positionS()).norme();


        // // cases: N1S2 (N3/S3):
        // if (N1S2 < N1N2 and N1S2 < S1S2 and N1S2 < S1N2) {
        //     if (S2N3 > S2S3) { return vm.magnet1; }
        //     else { return vm.magnet2; }
        // }
        // // cases: N1N2 (N3/S3)
        // else if (N1N2 < N1S2 and N1N2 < S1S2 and N1N2 < S1N2) {
        //     if (N2N3 > N2S3) { return vm.magnet3;  }
        //     else { return -1; }
        // }
        //
        // // cases: S1S2 (N3/S3):
        // else if (S1S2 < N1S2 and S1S2 < N1N2 and S1S2 < S1N2) {
        //     if (S2N3 > S2S3) { return -2; }
        //     else { return vm.magnet3; }
        // }
        // // cases: S1N2 (N3/S3)
        // else {
        //     if (S1N2 < N1S2 and S1N2 < N1N2 and S1N2 < S1S2) { return vm.magnet2; }
        //     else { return vm.magnet1; }
        // }

        // cases: P1S2 P2N1:
        if ((P1S2 > P1N2 and P1S3 > P1N3 and P2N1 > P2S1) or  (P1S2 < P1N2 and P1S3 < P1N3 and P2N1 < P2S1)) {
            return vm.magnet1;
        }

        if ((P1N2 > P1S2 and P1S3 > P1N3 and P2N1 > P2S1) or  (P1N2 < P1S2 and P1S3 < P1N3 and P2N1 < P2S1)) {
            return vm.magnet3;
        }

        if ((P1S2 > P1N2 and P1N3 > P1S3 and P2N1 > P2S1) or  (P1S2 < P1N2 and P1N3 < P1S3 and P2N1 < P2S1)) {
            return vm.magnet2;
        }

        return -1;
    } // sends back oddmagnet index

    //Method to find the type of a double vertex 3=ff, 2=fw, 1=wf, 0=ww
    int doublevertexstrength(doublevertex dv)
    {
        if (Oddoneout(dv.v1) == dv.centralmagnet and Oddoneout(dv.v2) == dv.centralmagnet) {
            return 3;
        } // ff
        else if (Oddoneout(dv.v1) != dv.centralmagnet and Oddoneout(dv.v2) == dv.centralmagnet) {
            return 2; // fw
        }
        else if (Oddoneout(dv.v1) == dv.centralmagnet and Oddoneout(dv.v2) !=
          dv.centralmagnet)
        {
            return 1; // wf
        }
        else if (Oddoneout(dv.v1) != dv.centralmagnet and Oddoneout(dv.v2) !=
          dv.centralmagnet)
        {
            return 0; // ww
        }

        return -1;
    }

    //Method that returns a boolean that indicates if a magnet/ double vetex is pointing inwards or outwards
    bool doublevertexup(doublevertex dv, int magnetindex)
    {
        Vecteur3D barycenterofdv = s.tab_ptr_Magnets[dv.centralmagnet]->position;
        Vecteur3D upwardOrientation(barycenterofdv - dode.position);
        double seperationAfterorientation1 =
          (s.tab_ptr_Magnets[magnetindex]->position
          + s.tab_ptr_Magnets[magnetindex]->orientationV() * s.tab_ptr_Magnets[magnetindex]->length / 2
          - barycenterofdv ).norme();
        double seperationAfterorientation2 = // reorienting the central magnet orientation
          (-1 * s.tab_ptr_Magnets[magnetindex]->position
          + s.tab_ptr_Magnets[dv.centralmagnet]->orientationV() * s.tab_ptr_Magnets[magnetindex]->length / 2
          + barycenterofdv ).norme();
        double seperationBetweencenters =
          (s.tab_ptr_Magnets[magnetindex]->position
          - barycenterofdv ).norme();
        int fixer = 1;
        if (seperationAfterorientation1 < seperationBetweencenters) { fixer *= -1; } // fixing branch magnet

        if (seperationAfterorientation2 > seperationBetweencenters) { fixer *= -1; } // fixing centralmagnet

        if (fixer
          * ((s.tab_ptr_Magnets[magnetindex]->orientationV() ^ s.tab_ptr_Magnets[dv.centralmagnet]->orientationV())
          * upwardOrientation) > 0) { return 1; }

        return 0;
    }

    //Given a magnet's index in the magnets table (array) finds its corresponding double vertex in the double vertex table
    size_t finddoublevertex(int magnetindex)
    {
        for (size_t i = 0; i < DV.size(); i++) {
            if (DV[i].centralmagnet == magnetindex) {  return i; }
        }
        return -1;
    }


    // This method basically treats fw and wf the same. ff= 4,fw=3,wf=3, ww=1
    int doublevertexstrengthRelative(doublevertex dv, int magnetindex)
    {
        int strength = 0;
        int doublevertexindexofmagnet = finddoublevertex(magnetindex);
        if (doublevertexstrength(DV[doublevertexindexofmagnet]) == 0 //ww
          or doublevertexstrength(DV[doublevertexindexofmagnet]) == 3) //ff
        {
            strength = doublevertexstrength(DV[doublevertexindexofmagnet]);
        }
        else {
            if (Oddoneout(dv.v1) == magnetindex) {
                strength = 2; //fw
            }
            else if (Oddoneout(dv.v2) == magnetindex) {
                strength = 2; //wf
            }
            else { strength = 1; }
        }

        return strength + 1;
    }

    //Given a vertexmagnet returns an array of the indices of the 2 magnets (out of 3) are not the centrat magnet of a given double vertex
    std::vector<int> nonCentralindex(doublevertex dv, vertexmagnets vm)
    {
        std::vector<int> indices;
        if (vm.magnet1 != dv.centralmagnet) { indices.push_back(vm.magnet1); }

        if (vm.magnet2 != dv.centralmagnet) { indices.push_back(vm.magnet2); }

        if (vm.magnet3 != dv.centralmagnet) { indices.push_back(vm.magnet3); }

        return indices;
    }

    //Complicated method, every digit represents the relative strength of agiven magnet in a double vertex, returns a five digit number (for each of the five magnets).
    int doublevertexstrengthDetailed(doublevertex dv)
    {
        int detailed((doublevertexstrength(dv) + 1) * 100);
        std::vector<int> indices1 = nonCentralindex(dv, dv.v1);
        if (doublevertexup(dv, indices1[0])) {
            // v1m1
            detailed += doublevertexstrengthRelative(dv, indices1[0]);
            // v1m2
            detailed += doublevertexstrengthRelative(dv, indices1[1]) * 10;
        }
        else {
            // v1m1
            detailed += doublevertexstrengthRelative(dv, indices1[0]) * 10;
            // v1m2
            detailed += doublevertexstrengthRelative(dv, indices1[1]);
        }

        std::vector<int> indices2 = nonCentralindex(dv, dv.v2);
        if (doublevertexup(dv, indices2[0])) {
            // v2m1
            detailed += doublevertexstrengthRelative(dv, indices2[0]) * 1000;
            // v2m2
            detailed += doublevertexstrengthRelative(dv, indices2[1]) * 10000;
        }
        else {
            // v2m1
            detailed += doublevertexstrengthRelative(dv, indices2[0]) * 10000;
            // v2m2
            detailed += doublevertexstrengthRelative(dv, indices2[1]) * 1000;
        }

        return detailed;
    }

    // Energies:

    double vertexEnergy(vertexmagnets vm)
    {
        double Energy = 0;
        Energy += s.tab_ptr_Magnets[vm.magnet1]->spotB(s.tab_ptr_Magnets[vm.magnet2]);
        Energy += s.tab_ptr_Magnets[vm.magnet1]->spotB(s.tab_ptr_Magnets[vm.magnet3]);
        Energy += s.tab_ptr_Magnets[vm.magnet2]->spotB(s.tab_ptr_Magnets[vm.magnet3]);
        return Energy;
    }

    double doublevertexEnergy(doublevertex dv)
    {
        return vertexEnergy(dv.v1) + vertexEnergy(dv.v2);
    }

    double doublevertexEnergyf(doublevertex dv)
    {   double Energy = 0;
        if (dv.v1.magnet1 != dv.centralmagnet) {
            Energy += s.tab_ptr_Magnets[dv.centralmagnet]->
              spotB(s.tab_ptr_Magnets[dv.v1.magnet1]);
        }

        if (dv.v1.magnet2 != dv.centralmagnet) {
            Energy += s.tab_ptr_Magnets[dv.centralmagnet]->
              spotB(s.tab_ptr_Magnets[dv.v1.magnet2]);
        }

        if (dv.v1.magnet3 != dv.centralmagnet) {
            Energy += s.tab_ptr_Magnets[dv.centralmagnet]->
              spotB(s.tab_ptr_Magnets[dv.v1.magnet3]);
        }

        if (dv.v2.magnet1 != dv.centralmagnet) {
            Energy += s.tab_ptr_Magnets[dv.centralmagnet]->
              spotB(s.tab_ptr_Magnets[dv.v2.magnet1]);
        }

        if (dv.v2.magnet2 != dv.centralmagnet) {
            Energy += s.tab_ptr_Magnets[dv.centralmagnet]->
              spotB(s.tab_ptr_Magnets[dv.v2.magnet2]);
        }

        if (dv.v2.magnet3 != dv.centralmagnet) {
            Energy += s.tab_ptr_Magnets[dv.centralmagnet]->
              spotB(s.tab_ptr_Magnets[dv.v2.magnet3]);
        }
        return Energy;
    }

    double faceEnergy1(facemagnets fm)
    {
        double Energy = 0;
        for (size_t i = 0; i < fm.doublevertexVector.size(); i++) {
            Energy +=
              s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->
              spotB(s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % fm.doublevertexVector.size()].centralmagnet]);
        }
        return Energy;
    }

    double faceEnergy(facemagnets fm)
    {
        double Energy = 0;
        for (size_t i = 0; i < fm.doublevertexVector.size(); i++) {
            for (size_t j = i + 1; j < fm.doublevertexVector.size(); j++) {
                if (i != (j) % fm.doublevertexVector.size()) {
                    Energy +=
                      s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->
                      spotB(s.tab_ptr_Magnets[fm.doublevertexVector[(j)
                        % fm.doublevertexVector.size()].centralmagnet]);
                }

                //                    std::cout << i <<"  " <<(j )
                //                                 % fm.doublevertexVector.size() << std::endl;
            }
        }
        return Energy;
    }

    double faceEnergyf(facemagnets fm)
    {
        double Energy = 0;
        for (size_t i = 0; i < fm.doublevertexVector.size(); i++) {
            if (fm.doublevertexVector[i].v1.magnet1 != fm.doublevertexVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doublevertexVector[i
                    ].v1.magnet1]);
            }

            if (fm.doublevertexVector[i].v1.magnet2 != fm.doublevertexVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doublevertexVector[i
                    ].v1.magnet2]);
            }

            if (fm.doublevertexVector[i].v1.magnet3 != fm.doublevertexVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doublevertexVector[i
                    ].v1.magnet3]);
            }

            if (fm.doublevertexVector[i].v2.magnet1 != fm.doublevertexVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doublevertexVector[i
                    ].v2.magnet1]);
            }

            if (fm.doublevertexVector[i].v2.magnet2 != fm.doublevertexVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doublevertexVector[i
                    ].v2.magnet2]);
            }

            if (fm.doublevertexVector[i].v2.magnet3 != fm.doublevertexVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doublevertexVector[i
                    ].v2.magnet3]);
            }
        }
        return Energy;
    } // faceEnergyf

    // Returns a vector in the orientation of a face (the normal vector), depending on the direction the magnets on the face are pointing.
    std::vector<double> getFaceOrientaion(facemagnets fm)
    {
        // we can do it with vector product; seems elegant?
        std::vector<double> scalarproducts;
        size_t facesize = fm.doublevertexVector.size();
        for (size_t i = 0; i < facesize; ++i) {
            double scalarp(0);
            double P1N2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->position
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double P1S2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->position
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            double N1N2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->positionN()
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double S1N2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->positionS()
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double S1S2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->positionS()
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            double N1S2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->positionN()
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            if (P1N2 > P1S2 and S1S2 > N1S2) { scalarp = 1; }

            else if (P1S2 > P1N2 and N1N2 > S1N2) { scalarp = 1; }

            scalarproducts.push_back(scalarp);
        }
        return scalarproducts;
    } // getFaceOrientaion

    int getFaceOrientaionN(facemagnets fm)
    {
        // we can do it with vector product; seems elegant?
        std::vector<double> scalarproducts;
        Vecteur3D barycenterofface(0, 0, 0);
        size_t facesize = fm.doublevertexVector.size();
        for (size_t i = 0; i < facesize; ++i) {
            double scalarp(0);
            double P1N2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->position
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double P1S2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->position
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            double N1N2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->positionN()
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double S1N2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->positionS()
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double S1S2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->positionS()
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            double N1S2 = (s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->positionN()
              - s.tab_ptr_Magnets[fm.doublevertexVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            if (P1N2 > P1S2 and S1S2 > N1S2) { scalarp = 1; }

            else if (P1S2 > P1N2 and N1N2 > S1N2) { scalarp = 1; }

            scalarproducts.push_back(scalarp);
            barycenterofface += s.tab_ptr_Magnets[fm.doublevertexVector[i].centralmagnet]->position;
        }

        barycenterofface = barycenterofface / facesize;
        int streaksize     = 0;
        int maxstreaksize  = 1;
        int maxstreakindex = 0;
        for (size_t i = 0; i < 2 * scalarproducts.size(); ++i) {
            if (scalarproducts[i % scalarproducts.size()] == 1) {
                streaksize += 1;
            }

            if (streaksize >= maxstreaksize) {
                maxstreaksize  = streaksize;
                maxstreakindex = i;
            }

            if (scalarproducts[i % scalarproducts.size()] == 0) {
                streaksize = 0;
            }
        }
        // for orientation:
        Vecteur3D upwardorientation(barycenterofface - dode.position);
        double orientation =
          (s.tab_ptr_Magnets[fm.doublevertexVector[maxstreakindex % facesize].centralmagnet]->orientationV()
          ^ s.tab_ptr_Magnets[fm.doublevertexVector[(maxstreakindex + 1 + facesize)
          % facesize].centralmagnet]->orientationV())
          * upwardorientation;
        // fixer:
        int fixer = 1;

        double P1N2 = (s.tab_ptr_Magnets[fm.doublevertexVector[maxstreakindex % facesize].centralmagnet]->position
          - s.tab_ptr_Magnets[fm.doublevertexVector[(maxstreakindex + 1 + facesize)
          % facesize].centralmagnet]->positionN()).norme();
        double P1S2 = (s.tab_ptr_Magnets[fm.doublevertexVector[maxstreakindex % facesize].centralmagnet]->position
          - s.tab_ptr_Magnets[fm.doublevertexVector[(maxstreakindex + 1 + facesize)
          % facesize].centralmagnet]->positionS()).norme();
        if (P1N2 > P1S2) { fixer = -1; }

        if (orientation > 0) {
            return fixer * maxstreaksize;
        }
        else {
            return -1 * fixer * maxstreaksize;
            // % (facesize + 1)
        }
    } // getFaceOrientaion

    int numberoftype(facemagnets fm, int type)
    {
        int numberoftype = 0;
        for (auto& dv :fm.doublevertexVector) {
            int str = doublevertexstrength(dv);
            if ((type == 1) and (str == 1 or str == 2 )) { numberoftype++; }

            if ((type == 2) and (str == 3 )) { numberoftype++; }

            if ((type == 0) and (str == 0 )) { numberoftype++; }
        }
        return numberoftype;
    }

    int dodectype()
    {
        int dodectype = 0;
        for (auto& fm: FM) {
            int fmtype = getFaceOrientaionN(fm);
            switch (fmtype) {
                case 10: dodectype += 1;
                    break;
                case -10: dodectype += 10;
                    break;
                case 3: dodectype += 100;
                    break;
                case -3: dodectype += 1000;
                    break;
                case 2: dodectype += 10000;
                    break;
                case -2: dodectype += 100000;
                    break;
                case 1: dodectype += 1000000;
                    break;
                case -1: dodectype += 10000000;
                    break;
            }
        }
        return dodectype;
    }

    int dodectype1(int facetype)
    {
        int dodectype = 0;
        for (auto& fm: FM) {
            int fmtype = getFaceOrientaionN(fm);
            if (fmtype == facetype) { dodectype++; }
            else if (fmtype == -1 * facetype) {
                dodectype += 10;
            }
        }
        return dodectype;
    }
};
