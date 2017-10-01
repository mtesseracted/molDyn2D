# molDyn2D
molecular dynamics in 2D

## Dependencies and Running:
This is a basic program to simulate particles in 2 dimensions under
the influence of a Lennard-Jones type potential and make a movie of
the molecular dynamics. It requires g++ to compile the molecular 
dynamics simulation, gnuplot to make the frames of the movie, and 
mencoder to string the frames into a movie. To run:

%>bash run.sh

##Adjusting Inputs
To adjust the simulation settings you can change the values in the 
inputs file.

### Notes on changing inputs:
&nbsp;&nbsp;&nbsp;particles:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The simulation itself is fairly fast and can handle upwards
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;of 1k particles.  The plotting method however is very crude
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;and as such attempting to make a movie with over 128 particles
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;can take over 24 CPU hours.

&nbsp;&nbsp;&nbsp;particles/length:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;It's not recommended to set the density less than 1 particle per
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unit length^2.  The Force caculation is found such that it will
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;not calculate forces that result in numerical instabilities.  

&nbsp;&nbsp;&nbsp;vel0 & fps
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;If the vel0 is low (<2.0), it's recommended to set fps higher so
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;the movie will evolve faster.  If vel0 is high (>5.0), it's
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;recommeded to set the fps lower so you can see what's happening.
