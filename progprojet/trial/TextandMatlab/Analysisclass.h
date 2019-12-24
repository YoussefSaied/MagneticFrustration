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


struct vertixmagnets {
    Vecteur3D position;
    int       magnet1; // index
    int       magnet2; // index
    int       magnet3; // index
};

struct doublevertix {
    int           centralmagnet;
    vertixmagnets v1;
    vertixmagnets v2;
};

struct facemagnets {
    std::vector<doublevertix> doubleVertixVector;
    double                    orientation;
};


class SystemeDodec {
public:
    // constructor:
    SystemeDodec(std::string finalangles = "finalangles.in") : dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0,
          1.23607), false)
    {
        // VM initialisation:

        std::vector<std::vector<Vecteur3D> > vertixpositions = dode.vertipositions();


        for (size_t i = 0; i < vertixpositions.size(); ++i) {
            size_t si = (vertixpositions[i]).size();
            for (size_t j = 0; j < si; ++j) {
                int exists = -1;
                for (size_t l = 0; l < VM.size(); ++l) {
                    if (vertixpositions[i][j] == VM[l].position) { exists = l; }
                }
                if ((exists == -1)) {
                    VM.push_back({ vertixpositions[i][j], -1, -1, -1 });
                }
            }
        }

        for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
            size_t si = ((dode.vertipositions())[i]).size();
            for (size_t j = 0; j < si; ++j) {
                Vecteur3D pos( ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2);
                Vecteur3D polaraxis = (dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si];
                Vecteur3D v2        =
                  ((dode.vertipositions())[i][(j + 1) % si] - (dode.vertipositions())[i][(j + 2) % si]);
                Vecteur3D v3  = polaraxis ^ v2;
                double alph1  = -1 * atan(2) / 2;
                Vecteur3D v4  = v3.rotate(alph1, polaraxis);
                Vecteur3D axe = (v4 ^ polaraxis).normalise();

                Magnet M(pos, axe, 1, 0, polaraxis); // position pos, axis axe, movable yes, angle_0 0, polaraxis

                int index;
                // if (s.addMagnet(M)  { index = (s.tab_ptr_Magnets.size() - 1); }
                index = s.addMagnet(M);

                // VM:
                for (auto& vm: VM) {
                    if (((vm).position - (dode.vertipositions())[i][(j) % si]).norme() < 0.01) {
                        // cout<<((vm).position - (dode.vertipositions())[i][(j) % si]).norme()<<endl;
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

                    // if (((vm).position - (dode.vertipositions())[i][(j + 1) % si]).norme() < 0.01) {
                    //     if (vm.magnet1 == -1 and vm.magnet1 != index and vm.magnet2 != index and vm.magnet3 != index) { vm.magnet1 = index; }
                    //     else if (vm.magnet2 == -1) {
                    //         vm.magnet2 = index;
                    //     }
                    //     else if (vm.magnet3 == -1) {
                    //         vm.magnet3 = index;
                    //     }
                    // }
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
        for (size_t i = 0; i < vertixpositions.size(); ++i) {
            std::vector<doublevertix> VectorDVelem;
            size_t si = (vertixpositions[i]).size();
            for (size_t j = 0; j < si; ++j) {
                for (auto DVelem: DV) {
                    if (DVelem.v1.position == vertixpositions[i][j] and DVelem.v2.position ==
                      vertixpositions [i][(j + 1) % si]) { VectorDVelem.push_back(DVelem); }

                    if (DVelem.v2.position == vertixpositions[i][j] and DVelem.v1.position ==
                      vertixpositions [i][(j + 1) % si]) { VectorDVelem.push_back(DVelem); }
                }
            }
            FM.push_back({ VectorDVelem, -10 });
        }

        // set angles:
        s.setangles(finalangles);
    }

    // functions and structures and variables:


    Dodec dode;
    // The system:
    Systeme s;

    // functions and structures are easily generalisable to all solids, would be better with classes


    // tables:
    std::vector<vertixmagnets> VM;
    std::vector<doublevertix> DV;
    std::vector<facemagnets> FM;


    // Methods:

    // Vertix strength (f/w):

    std::string OddoneoutText(vertixmagnets vm)
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

        // angles:


        // return -1;
    } // sends back oddmagnet index

    int Oddoneout(vertixmagnets vm)
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

        // angles:


        return -1;
    } // sends back oddmagnet index

    int doublevertixstrength(doublevertix dv)
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

    bool doublevertixup(doublevertix dv, int magnetindex)
    {
        Vecteur3D barycenterofdv = s.tab_ptr_Magnets[dv.centralmagnet]->position;
        Vecteur3D upwardOrientation(barycenterofdv - dode.position);
        double seperationAfterorientation1 =
          (s.tab_ptr_Magnets[magnetindex]->position
          + s.tab_ptr_Magnets[magnetindex]->orientation() * s.tab_ptr_Magnets[magnetindex]->length / 2
          - barycenterofdv ).norme();
        double seperationAfterorientation2 = // reorienting the central magnet orientation
          (-1 * s.tab_ptr_Magnets[magnetindex]->position
          + s.tab_ptr_Magnets[dv.centralmagnet]->orientation() * s.tab_ptr_Magnets[magnetindex]->length / 2
          + barycenterofdv ).norme();
        double seperationBetweencenters =
          (s.tab_ptr_Magnets[magnetindex]->position
          - barycenterofdv ).norme();
        int fixer = 1;
        if (seperationAfterorientation1 < seperationBetweencenters) { fixer *= -1; } // fixing branch magnet

        if (seperationAfterorientation2 > seperationBetweencenters) { fixer *= -1; } // fixing centralmagnet

        if (fixer
          * ((s.tab_ptr_Magnets[magnetindex]->orientation() ^ s.tab_ptr_Magnets[dv.centralmagnet]->orientation())
          * upwardOrientation) > 0) { return 1; }

        return 0;
    }

    size_t finddoublevertix(int magnetindex)
    {
        for (size_t i = 0; i < DV.size(); i++) {
            if (DV[i].centralmagnet == magnetindex) {  return i; }
        }
        return -1;
    }

    int doublevertixstrengthRelative(doublevertix dv, int magnetindex)
    {
        int strength = 0;
        int doublevertixindexofmagnet = finddoublevertix(magnetindex);
        if (doublevertixstrength(DV[doublevertixindexofmagnet]) == 0
          or doublevertixstrength(DV[doublevertixindexofmagnet]) == 3)
        {
            strength = doublevertixstrength(DV[doublevertixindexofmagnet]);
        }
        else {
            if (Oddoneout(dv.v1) == magnetindex) {
                strength = 2;
            }
            else if (Oddoneout(dv.v2) == magnetindex) {
                strength = 2;
            }
            else { strength = 1; }
        }

        return strength + 1;
    }

    std::vector<int> nonCentralindex(doublevertix dv, vertixmagnets vm)
    {
        std::vector<int> indices;
        if (vm.magnet1 != dv.centralmagnet) { indices.push_back(vm.magnet1); }

        if (vm.magnet2 != dv.centralmagnet) { indices.push_back(vm.magnet2); }

        if (vm.magnet3 != dv.centralmagnet) { indices.push_back(vm.magnet3); }

        return indices;
    }

    int doublevertixstrengthDetailed(doublevertix dv)
    {
        int detailed((doublevertixstrength(dv) + 1) * 100);
        std::vector<int> indices1 = nonCentralindex(dv, dv.v1);
        if (doublevertixup(dv, indices1[0])) {
            // v1m1
            detailed += doublevertixstrengthRelative(dv, indices1[0]);
            // v1m2
            detailed += doublevertixstrengthRelative(dv, indices1[1]) * 10;
        }
        else {
            // v1m1
            detailed += doublevertixstrengthRelative(dv, indices1[0]) * 10;
            // v1m2
            detailed += doublevertixstrengthRelative(dv, indices1[1]);
        }

        std::vector<int> indices2 = nonCentralindex(dv, dv.v2);
        if (doublevertixup(dv, indices2[0])) {
            // v2m1
            detailed += doublevertixstrengthRelative(dv, indices2[0]) * 1000;
            // v2m2
            detailed += doublevertixstrengthRelative(dv, indices2[1]) * 10000;
        }
        else {
            // v2m1
            detailed += doublevertixstrengthRelative(dv, indices2[0]) * 10000;
            // v2m2
            detailed += doublevertixstrengthRelative(dv, indices2[1]) * 1000;
        }

        return detailed;
    }

    // energies:
    double vertixEnergy(vertixmagnets vm)
    {
        double Energy = 0;
        Energy += s.tab_ptr_Magnets[vm.magnet1]->spotB(s.tab_ptr_Magnets[vm.magnet2]);
        Energy += s.tab_ptr_Magnets[vm.magnet1]->spotB(s.tab_ptr_Magnets[vm.magnet3]);
        Energy += s.tab_ptr_Magnets[vm.magnet2]->spotB(s.tab_ptr_Magnets[vm.magnet3]);
        return Energy;
    }

    double doublevertixEnergy(doublevertix dv)
    {
        return vertixEnergy(dv.v1) + vertixEnergy(dv.v2);
    }

    double doublevertixEnergyf(doublevertix dv)
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
        for (size_t i = 0; i < fm.doubleVertixVector.size(); i++) {
            Energy +=
              s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
              spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % fm.doubleVertixVector.size()].centralmagnet]);
        }
        return Energy;
    }

    double faceEnergy(facemagnets fm)
    {
        double Energy = 0;
        for (size_t i = 0; i < fm.doubleVertixVector.size(); i++) {
            for (size_t j = i + 1; j < fm.doubleVertixVector.size(); j++) {
                if (i != (j) % fm.doubleVertixVector.size()) {
                    Energy +=
                      s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
                      spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[(j)
                        % fm.doubleVertixVector.size()].centralmagnet]);
                }

                //                    std::cout << i <<"  " <<(j )
                //                                 % fm.doubleVertixVector.size() << std::endl;
            }
        }
        return Energy;
    }

    double faceEnergyf(facemagnets fm)
    {
        double Energy = 0;
        for (size_t i = 0; i < fm.doubleVertixVector.size(); i++) {
            if (fm.doubleVertixVector[i].v1.magnet1 != fm.doubleVertixVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[i
                    ].v1.magnet1]);
            }

            if (fm.doubleVertixVector[i].v1.magnet2 != fm.doubleVertixVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[i
                    ].v1.magnet2]);
            }

            if (fm.doubleVertixVector[i].v1.magnet3 != fm.doubleVertixVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[i
                    ].v1.magnet3]);
            }

            if (fm.doubleVertixVector[i].v2.magnet1 != fm.doubleVertixVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[i
                    ].v2.magnet1]);
            }

            if (fm.doubleVertixVector[i].v2.magnet2 != fm.doubleVertixVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[i
                    ].v2.magnet2]);
            }

            if (fm.doubleVertixVector[i].v2.magnet3 != fm.doubleVertixVector[i].centralmagnet) {
                Energy += s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
                  spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[i
                    ].v2.magnet3]);
            }
        }
        return Energy;
    } // faceEnergyf

    // orientation of face
    std::vector<double> getFaceOrientaion(facemagnets fm)
    {
        // we can do it with vector product; seems elegant?
        std::vector<double> scalarproducts;
        size_t facesize = fm.doubleVertixVector.size();
        for (size_t i = 0; i < facesize; ++i) {
            double scalarp(0);
            double P1N2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->position
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double P1S2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->position
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            double N1N2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->positionN()
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double S1N2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->positionS()
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double S1S2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->positionS()
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            double N1S2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->positionN()
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
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
        size_t facesize = fm.doubleVertixVector.size();
        for (size_t i = 0; i < facesize; ++i) {
            double scalarp(0);
            double P1N2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->position
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double P1S2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->position
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            double N1N2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->positionN()
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double S1N2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->positionS()
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionN()).norme();
            double S1S2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->positionS()
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            double N1S2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->positionN()
              - s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->positionS()).norme();
            if (P1N2 > P1S2 and S1S2 > N1S2) { scalarp = 1; }

            else if (P1S2 > P1N2 and N1N2 > S1N2) { scalarp = 1; }

            scalarproducts.push_back(scalarp);
            barycenterofface += s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->position;
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
          (s.tab_ptr_Magnets[fm.doubleVertixVector[maxstreakindex % facesize].centralmagnet]->orientation()
          ^ s.tab_ptr_Magnets[fm.doubleVertixVector[(maxstreakindex + 1 + facesize)
          % facesize].centralmagnet]->orientation())
          * upwardorientation;
        // fixer:
        int fixer = 1;

        double P1N2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[maxstreakindex % facesize].centralmagnet]->position
          - s.tab_ptr_Magnets[fm.doubleVertixVector[(maxstreakindex + 1 + facesize)
          % facesize].centralmagnet]->positionN()).norme();
        double P1S2 = (s.tab_ptr_Magnets[fm.doubleVertixVector[maxstreakindex % facesize].centralmagnet]->position
          - s.tab_ptr_Magnets[fm.doubleVertixVector[(maxstreakindex + 1 + facesize)
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
        for (auto& dv :fm.doubleVertixVector) {
            int str = doublevertixstrength(dv);
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
