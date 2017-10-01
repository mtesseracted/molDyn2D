#include<stdio.h>
#include<cstdlib>
#include<cmath>
#include<string>

#include"particleBox.h"

#ifndef DELT
#define DELT 0.001
#endif
#ifndef TIME
#define TIME 15
#endif

int main(int argc, char **argv)
{
    //Initialize program variables from command line
    if(argc < 4) {
	printf("Usage:>%s <side_len> <num_particles> <velocity> [timestep]\n", argv[0]);
	exit(1);
    }
    int sides = strtol(argv[1],NULL,10);
    int size = strtol(argv[2],NULL,10);
    double temp = strtof(argv[3],NULL);
    double timestep;
    if(argc >= 5) {
	timestep = strtof(argv[4],NULL);
    } else {
	timestep = DELT;
    }
    int printSteps = 20;
    int runs = (int)((double)TIME / timestep);
#ifdef HEAT
    int heatSteps = 0;
    double factor = HEAT;	//heating factor
    double equil = 5.0;		//how many seconds to wait for equilibrium
#endif

    //Initialize particle box and relevant quantities to track
    particleBox *box = new particleBox(size, sides, timestep);
    double vel2;		//velocity^2 
    double velx, vely;		//velocity x & y
    double tvelx, tvely;	//total velocity components
    double disp2;		//displacement^2
    double dispx, dispy; 	//displacement x & y
    double pot;	  		//potential energy
    double energy;		//total energy
    double prevE = 0.0, delE;	//previous energy & change in energy
    double N = (double)size;

    //Open files for printing position and velocities
    std::string fvels = "Vels.dat";	//print all velocities in one column to make histogram later
    std::string fpos = "Pos.dat";	//print each particle in its own column
    FILE *pFile;
    pFile = fopen(fpos.c_str(),"w");
    FILE *vFile;
    vFile = fopen(fvels.c_str(),"w");

    //Print headers to stdout results file
    printf("Side Length: %d, Particles: %d, time-step: %f, V0: %f, Run Time: %d\n", 
	    sides, size, timestep, temp, (int)TIME);
    printf("Step: \ttDispX:  \t tDispY:  \ttDisp2:  \ttVelX:  \ttVelY:  \ttVel2/2:  \tPotential: \tNet E:   \tDelta E:  \n");
    //print position header
    for(int i=0; i<size; i++)
    {
	fprintf(pFile,"P%dx   \tP%dy   \t",i,i);
    }
    fprintf(pFile,"\n");

    box->initPos(false);		//initialize with even spread throughout box
    box->initVel(-temp/2.0,temp/2.0);	//intialize velocities
    box->firstStep();			//take first step

#ifdef DEBUG
    printf("DEBUG MODE ENABLED\n");
    int steps = 0;
    char buffer[80];
#endif

    for(int i=0; i<runs; i++)
    {
#ifdef DEBUG		//Debug CLI
	if(steps == 0) {
	    buffer[79] = 0;		//logic placeholder to loop until steps are taken
	    while(buffer[79] == 0) {
		printf("\nOptions: Print Forces [f], Positions [p], Velocities [v], or Step [num of steps]\n");
		scanf("%s", buffer);
		if(buffer[0] == 'f') {
		    printf("X Forces\n"); box->printForces(box->xforces, size);
		    printf("Y Forces\n"); box->printForces(box->yforces, size);
		    printf("N Forces\n"); box->printForces(box->nforces, size);
		}else if(buffer[0] == 'p') {
		    printf("Positions with vectors:\n"); box->printPos();
		}else if(buffer[0] == 'v') {
		    printf("Velocities: \n"); box->printVels(box->parts, size);
		}else {
		    steps = strtol(buffer,NULL, 10);
		    if(steps <= 0) { 
			printf("Invalid command, running 1 step\n"); 
			steps=1;
		    }
		    buffer[79] = 1;
		}
	    }
	}
	steps--;
#endif	
	//Print output to files
	if( (i%printSteps) == 0 || (i < printSteps) ){
            for(int j=0; j<size; j++){	
	        tvelx = tvely = 0.0;
	        fprintf(pFile,"%7.4e\t%7.4e\t", box->parts[j].xp1, 
			box->parts[j].yp1);
		velx = box->getXVel(j); vely = box->getYVel(j); 
		tvelx += velx; tvely += vely;
	
		fprintf(vFile,"%7.4e\t%7.4e\t%7.4e\n", 
			sqrt(box->getVel2(j)),velx,vely);
	    }
	    fprintf(pFile,"\n");
            if( (i % (10*printSteps)) == 0) fprintf(stderr, "\n"); 
	    fprintf(stderr,"%d ", i);

	    dispx = box->getTXDisp()/N;
	    dispy = box->getTYDisp()/N;
	    disp2 = box->getTDisp2()/N;
	    vel2 = box->getTVel2()*0.5;
	    pot = box->getTPot();
	    energy = vel2 + pot;
	    delE = energy - prevE;
	    printf("%d\t%7.4e\t%7.4e\t%7.4e\t%7.4e\t%7.4e\t%7.4e\t%7.4e\t%7.4e\t%7.4e\n",
		    i,dispx,dispy,disp2,tvelx,tvely,vel2,pot,energy,delE);
	    prevE = energy;
	}
#ifdef HEAT

	if( (i*timestep) > equil ) {
	    if(heatSteps <= 0) {
		box->heatBox(factor);
		heatSteps = (int)(equil/timestep);
	    }
	}
	heatSteps--;
#endif
	box->step();
    }  
   
    delete box;
    return 0;
}
