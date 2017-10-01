#ifndef PARTICLEBOX_H
#define PARTICLEBOX_H

#define TRUNC 3.0	//Truncate forces after this distance
#define EPS1 0.001	//minimum distance will calculate forces

#define CONST1 24.0
#define CONST2 4.0
#define SIG13 1.0
#define SIG12 1.0
#define SIG7 1.0
#define SIG6 1.0

typedef struct{
    double xp0, xp1;	//x position, now=1, previous=0
    int xcrosses;	//times crossed the x boundary
    double xv;		//x velocity
    double yp0, yp1;	//y position
    double yv;		//y velocity
    int ycrosses;
}particle;

//Particle Box class for particles interacting in
//  2D.  Potential used is Leonard-Jones with 
//  toroidal boundary conditions and potential
//  boundary condtions encircling the box, i.e.
//  boxes on the corners allowed to influence,
//  not just ones at right angles.  
class particleBox
{
public:
   particleBox(int size, int length, double timestep);
   ~particleBox();

   //Stat-getting functions
   double getVel2(int i);		//get individual square velocity
   double getTVel();			//get total velocity
   double getXVel(int i);
   double getYVel(int i);
   double getTVel2();			//get total square velocity
   double getDisp2(int i);		//get individual square displacement
   double getTDisp();			//get total displacement
   double getTXDisp();
   double getTYDisp();
   double getTDisp2();			//get total square displacement
   double getTPot();			//get total potential energy

   //setup and stepping functions
   void step();					//take a step
   void initVel(double min, double max);	//initialize velocities
   void initPos(bool random);			//initialize posisitions
   void firstStep();				//start first step
   void copy(particle source, particle *dest);	//read the name genius

   //printing functions
   void printInitPos();				//print intial position
   void printPos();				//print positions and motion vectors
   void printForces(double *f, int n);		//print once force matrix
   void printVels(particle *p, int n); 		//print all velocities

   //find forces between all particles, also fills in potentials
   double *findForces(double *n, double *x, double *y);
   double findForce(double rn1);	//get force for a given inverse distance r^-1
   double findPot(double rn1);		//get potential for a given inverse distance

   //Update particle positions and velocity (Verlet)
   void updateParts();			

   //find min distance between two particles
   double minDist(int i1, int i2, double *x, double *y);
   double findDist(particle p1, particle p2);

   //Move a particle by x and y then return that moved particle
   particle move(particle p, double x, double y);

   //heat box by increasing velocities by a factor > 1.0 (<1.0 == cooling)
   void heatBox(double factor);


//private:
    particle *parts;	//the particles being tracked
    particle *oparts;	//original particle states @ t=0
    int n;		//num of particles being tracked
    int l;		//length of side of box
    double delt;	//delta t, time-step
    double delt2;	//delta t**2 (save time)
    double *xforces;	//x forces (n*n size)
    double *yforces;	//y forces (n*n size)
    double *nforces;	//net forces (n*n size)
    double *potentials;	//potential energies between particles (n*n)
};

#endif
