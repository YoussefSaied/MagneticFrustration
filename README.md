# Dodecahedron molecular simulation

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)
This is a molecular dynamics simulation of a macrosopic magnetically frustrated system. The system simulated is a system of 30 magnets wach lying on an edge of a dodecahedron. I would recommend reading the article/ report before using it. *Insert link to article*

## Getting Started

These instructions will help you set the project up and running on your local machine for development and testing purposes.

### Prerequisistes
Although the simulation would work on any OS, it's highly recommended to use a Linux distro such as Ubuntu. Regardless, the following packages and programs are required:
 - [Qt](https://www.qt.io/download)

 - A C++ compiler (Included in Ubuntu)
 - OpenGL (Normally, Included in Ubuntu)
 - Armadillo C++ library
```sh
$ sudo apt-get install liblapack-dev
$ sudo apt-get install libblas-dev
$ sudo apt-get install libboost-dev
$ sudo apt-get install libarmadillo-dev
```
### IDE
I recommend using atom (with beautify and a linter) or VS Code or Qt Creator. 

## Functionalities
 - Graphically or textually simulate Magnets with one or two degrees of freedom based on the configuration choosen in Main:
    - DodecH for 2 degrees of freedom magnets
    - Dodec for 1 degree of freedom magnets
 - Save and load configurations.
 - Analyse and identifys clusters in groundstates using the Analysis program.
 - Change system dimensions and magnets' strength.

### Main Classes

- Magnet: An abstract class representing a magnet
- MagnetA: Single degree of freedom magnets.
- MagnetH: Two degree of freedom magnets.
- System: The class that contains the magnets (A vector of pointers to magnets). It also containg the numerical integrators which calculate the forces and angular velocities and deisplacements for all the magnets at every timestep.
- vue_opengl: The drawing class that contains the methods to draw everything.
- Window: The class that creates the program's window

There are many, many other classses.
## Text simulation
The text simulation uses the Qt project files simulationt.pro and TextandMatlab.pro.
It has two possible configuration modes:
- configA: the analysis configuration, which takes as input a file with the angles of a groundstate of a Dodec and outputs 4 files :
```
       Vertex: [Energy, anglex3] (x20)
       DVertex: [anglex5, Energy, Dtype] (x30)
       Face: [anglex5, Vtypex5, Ftype, Energy] (x12)
       Dodec: [# of different Ftype]

       Vtype = {ff,fw,ww}
       Dtype = {ww-ww-ff-ww-ww}
       Ftype = {5,4,3,2}
```
- configT: normal text simulation configuration, would either output one complete detailled simulation to a text file or optionally only the angles of the last configuration. It could also be used to run multiple simulations.

## Runme files
There are three runme files, usable on Linux/ Unix:
- runmegraphical.sh: this would build then launch the graphical simulation window.
- runmeAnalysis.sh: this would build then launch the analysis program which would then output the four text files previously mentioned.
- runmetext.sh: this would build then launch a text simulation.

## Suggestions for improvements and further research

- I would suggest trying to compare the behaviour of the magnets with the two degrees of freedom to the ones with just one degree of freedom. It would be interesting to see if the same clusters and relations still hold and what changes.
- The two degrees of freedom magnets simulation has stability issues, I suggest trying RK4.
- I would suggest varying the system's dimensions and seeing how things change. I did that and when the magnets' lengths cross a certain threshold the magnets' local interactions lose importance and new order emerges (could be possibly interpreted as a phase transition?). This transition is most visible for the two degrees of freedom magnets. 
- The Blender folder contains Blender created models for use if another graphical engine is used, for example using Unreal engine or Unity. I would personally recommend to keep using OpenGL and Qt as they are faster and more accurate. Otherwise Unreal engine would be easier to transition to.
- In the TextandMatlab folder there are two Matlab scripts that I would advise to use to load the data and generate graphs.


## Questions

I would be happy to answer any questions!


License
----

MIT






