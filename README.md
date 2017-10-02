# molDyn2D
molecular dynamics in 2D

## Dependencies and Running:
This is a basic program to simulate particles in 2 dimensions under
the influence of a Lennard-Jones type potential and make a movie of
the molecular dynamics. It requires bash to run the control script,
g++ to compile the molecular dynamics simulation, gnuplot to make 
the frames of the movie, and mencoder to string the frames into a
movie. To run:

%>bash run.sh

## Adjusting Inputs
To adjust the simulation settings you can change the values in the 
inputs file.

### Notes on changing inputs:
* particles:
  * The simulation itself is fairly fast and can handle upwards of 
  1k particles. The plotting method however is very crude and as such
  attempting to make a movie with over 128 particles can take over 24
  CPU hours.

* particles/length:
  * It's not recommended to set the density less than 1 particle per 
  unit length^2.  The Force caculation is found such that it will not
  calculate forces that result in numerical instabilities.  
  
* vel0 (intial velocity)
  * vel0 is the max possible initial velocity of each particle in 
  any given direction.  It is related to the temperature of the
  simulation by the Bolzmann distribution. For best results keep this
  a positive value less than 20. 

* fps (frames per second)
  * If the vel0 is low (<2.0), it's recommended to set fps higher so
  the movie will evolve faster.  If vel0 is high (>5.0), it's 
  recommeded to set the fps lower so you can see what's happening.
