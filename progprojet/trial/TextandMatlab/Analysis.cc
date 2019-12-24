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
#include "ConfigFile.tpp"
#include "Analysisclass.h"

using namespace std;

int main(int argc, char * argv[])
{
    /*
       I think this is the most logical way to do it:
       Rather than working out the angles from the indices in Matlab, we
       do it here so that we have 4 files:


       Vertex: [Energy, anglex3] (x20)
       DVertex: [anglex5, Energy, Dtype] (x30)
       Face: [anglex5, Vtypex5, Ftype, Energy] (x12)
       Dodec: [#of different Ftype]

       Vtype = ff,fw,ww
       Dtype = intx5 eg ww-ww-ff-ww-ww
       Ftype = 5,4,3,2
     */

    SystemeDodec SD("finalangles");


    // output files:
    string outputPath = "temp/";
    if (argc > 1) { // Output file specified by user
        outputPath = argv[1];
    }

    // create:
    //////Magnet: // Angle // Moment // Type
    string Magnet = outputPath + "Magnet";
    unique_ptr<ofstream> tfile0(new ofstream(Magnet.c_str()));

    ///////VERTEX: // Angles (x3) // Energy
    // comments:  Oddoneout gives index of forced magnet
    string Vertex = outputPath + "Vertex";
    unique_ptr<ofstream> tfile1(new ofstream(Vertex.c_str()));
    for (auto& vm: SD.VM) {
        *tfile1 << SD.vertixEnergy(vm) << " " << vm.magnet1 << " " << vm.magnet2 << " " << vm.magnet3 << endl;
    }

    ////DOUBLE VERTEX: // Angles (x5) // Energy // type
    // comments: type = int with 5 digits, each digit has value 3,2,1,0
    // digits correspond to magnets in double vertex (3rd digit is central mag)
    // values correspond to ff=3,fw=2,wf=1,ww=0
    string DVertex = outputPath + "DVertex";
    unique_ptr<ofstream> tfile2(new ofstream(DVertex.c_str()));
    for (auto& dv: SD.DV) {
        *tfile2 << dv.centralmagnet << " " << SD.doublevertixEnergyf(dv) << " "
                << SD.doublevertixstrengthDetailed(dv)
                << " " << dv.v1.magnet1 << " " << dv.v1.magnet2 << " " << dv.v1.magnet3
                << " " << dv.v2.magnet1 << " " << dv.v2.magnet2 << " " << dv.v2.magnet3
                << endl;
    }

    ////FACE: // Angles (x5) // orientation // Energy // types
    // comments: orientation gives max streak size = 5,4,3,2 (-1,1 for CW,ACW)
    // Y: We can use the index to get all the information we might need
    string Face = outputPath + "Faceparameters";
    unique_ptr<ofstream> tfile3(new ofstream(Face.c_str()));
    for (auto& fm: SD.FM) {
        for (auto& dv: fm.doubleVertixVector) {
            *tfile3 << dv.centralmagnet << " ";
        }
        std::vector<double> v = SD.getFaceOrientaion(fm);
        double FaceEnergy     = SD.faceEnergyf(fm);
        *tfile3 << SD.numberoftype(fm, 0) << " " << SD.numberoftype(fm, 1) << " " << SD.numberoftype(fm, 2) << " "
 // << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << " "
                << SD.getFaceOrientaionN(fm) << " "
                << FaceEnergy << " ";
        *tfile3 << endl;
    }


    string Dodec = outputPath + "Dodectype";
    unique_ptr<ofstream> tfile4(new ofstream(Dodec.c_str()));
    // *tfile4 << SD.dodectype();
    *tfile4 << SD.dodectype1(10) << " " << SD.dodectype1(3) << " " << SD.dodectype1(2) << " " << SD.dodectype1(1);

    // string MagnetsofFace = "FaceMagnetsindex";
    string Dvertexindex = "Dvertexindex";

    return 0;
} // main
