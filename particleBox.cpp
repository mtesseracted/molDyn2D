#include<cmath>
#include<stdio.h>
#include<cstdlib>
#include"particleBox.h"


particleBox::particleBox(int size, int length, double timestep)
{
    n = size;
    l = length;
    delt = timestep;
    delt2 = delt*delt;
    parts = new particle[n]();
    oparts = new particle[n]();
    xforces = new double[n*n];
    yforces = new double[n*n];
    nforces = new double[n*n];
    potentials = new double[n*n];

    for(int i=0; i<n; i++)
    {
	parts[i].xcrosses = parts[i].ycrosses = 0;
    }
}

particleBox::~particleBox()
{
    delete[] parts;
    delete[] oparts;
    delete[] xforces;
    delete[] yforces;
    delete[] nforces;
    delete[] potentials;
}

double particleBox::getVel2(int i)
{
    return(parts[i].xv*parts[i].xv + parts[i].yv*parts[i].yv);
}

double particleBox::getDisp2(int i)
{
    double oneL = (double)l;
    double xd = parts[i].xp1 - oparts[i].xp0;
    xd += oneL*parts[i].xcrosses;
    double yd = parts[i].yp1 - oparts[i].yp0; 
    yd += oneL*parts[i].ycrosses;
    return(xd*xd + yd*yd);
}

double particleBox::getTVel()
{
    double retVal = 0.0;
    for(int i=0; i<n; i++)
    {
	retVal += sqrt(getVel2(i));
    }
    return retVal;
}

double particleBox::getXVel(int i)
{
    return parts[i].xv;
}
double particleBox::getYVel(int i)
{
    return parts[i].yv;
}

double particleBox::getTVel2()
{
    double retVal = 0.0;
    for(int i=0; i<n; i++)
    {
	retVal += getVel2(i);
    }
    return retVal;
}

double particleBox::getTDisp()
{
    double retVal = 0.0;
    for(int i=0; i<n; i++)
    {
	retVal += sqrt(getDisp2(i));
    }
    return retVal;
}

double particleBox::getTXDisp()
{
    double retval = 0.0;
    double oneL = (double)l;
    for(int i=0; i<n; i++)
    {
	retval += parts[i].xp1 - oparts[i].xp0;
	retval += oneL*parts[i].xcrosses;
    }
    return retval;
}

double particleBox::getTYDisp()
{
    double retval = 0.0;
    double oneL = (double)l;
    for(int i=0; i<n; i++)
    {
	retval += parts[i].yp1 - oparts[i].yp0;
	retval += oneL*parts[i].ycrosses;
    }
    return retval;
}


double particleBox::getTDisp2()
{
    double retVal = 0.0;
    for(int i=0; i<n; i++)
    {
	retVal += getDisp2(i);
    }
    return retVal;
}

double particleBox::getTPot()
{
    double retval = 0.0;
    for(int i=0; i<n; i++)
    {
	for(int j=i+1; j<n; j++)
	{
	    retval += potentials[(i*n)+j];
	}
    }
    return retval;
}

void particleBox::initVel(double min, double max)
{
    double range = max - min;
    double xvt = 0.0;
    double yvt = 0.0;
    for(int i=1; i<n; i++)
    {
	parts[i].xv = ((double)rand() / (double)RAND_MAX)*range + min;
	xvt += parts[i].xv;
	parts[i].yv = ((double)rand() / (double)RAND_MAX)*range + min;
	yvt += parts[i].yv;
    }
    parts[0].xv = -xvt;
    parts[0].yv = -yvt;

#ifdef DEBUG    
    printf("Initial Velocities:\n");
    printVels(parts,n);
    printf("\n");
#endif
}

//initialize positions
//  random indicated whether randomly distributed (true)
//  or semi-evenly distributed (false)
void particleBox::initPos(bool random)
{
    if(random) {
	double oneL = (double)l;
	for(int i=0; i<n; i++)
	{
	    parts[i].xp0 = (double)rand() / (double)RAND_MAX * oneL;
	    parts[i].yp0 = (double)rand() / (double)RAND_MAX * oneL;
	}
    } else {
	double rootn = sqrt((double)n);		//root n
	double distro = (double)l / rootn;	//distribution spacing
	int delx;	//spacial count of distributions per side + 1

	if( sqrt(n) == floor(sqrt(n) )){	//check perfect square
	    delx = (int)rootn;				
	}else {
	    delx = (int)rootn + 1;
	}
	   
	double xc = distro / 2.0;	//x & y coordinates
	double yc = distro / 2.0;

	int index = 0;			//particle index
	for(int i=0; i<delx; i++)
	{
	    xc = distro / 2.0;
	    for(int j=1; j<=delx; j++)
	    {
		if(index < n){
		    parts[index].xp1 = xc;	//set p1 too for findForces
		    parts[index].xp0 = xc;
		    parts[index].yp1 = yc;
		    parts[index++].yp0 = yc;
                    xc += distro;
		} else {
		    break;
		}
	    }
	    yc += distro;
	}
    }
}
		

void particleBox::firstStep()
{   
    findForces(nforces, xforces, yforces);
#ifdef DEBUG
    printf("Initial Forces:\n");
    printf("X Forces\n");
    printForces(xforces,n);
    printf("Y Forces\n");
    printForces(yforces,n);
    printf("N Forces\n");
    printForces(nforces,n);
    printf("\n");
#endif

    for(int i=0; i<n; i++)
    {
	double netx = 0.0;	//net forces
	double nety = 0.0;
	for(int j=0; j<n; j++)
	{
	    if(j < i) {
		netx += xforces[(i*n)+j];
		nety += yforces[(i*n)+j];
	    } else if( j > i ) {
		netx += xforces[(i*n)+j];
		nety += yforces[(i*n)+j];
	    }
	}

	parts[i].xp1 = parts[i].xp0 + parts[i].xv*delt + 0.5*netx*delt2;
	parts[i].yp1 = parts[i].yp0 + parts[i].yv*delt + 0.5*nety*delt2;

	copy(parts[i], &oparts[i]);	//store original stats
    }

}

void particleBox::printInitPos()
{
    for(int i=0; i<n; i++)
    {
	printf("%f\t%f\n", parts[i].xp0, parts[i].yp0);
    }
}

void particleBox::printPos()
{
    double delx, dely;
    for(int i=0; i<n; i++)
    {
	delx = parts[i].xp1 - parts[i].xp0;
	delx *= 100;
	dely = parts[i].yp1 - parts[i].yp0;
	dely *= 100;
	printf("%f\t%f\t%f\t%f\n", parts[i].xp0, parts[i].yp0,delx,dely);
    }
}

void particleBox::printForces(double *forces, int n)
{
    for(int i=0; i<n; i++)
    {
	for(int j=0; j<n; j++)
	{
	    printf("%5.2e  ", forces[(i*n)+j]);
	}
	printf("\n");
    }
}

void particleBox::printVels(particle *parts, int n)
{
    printf("X vel,    Y vel\n");
    for(int i=0; i<n; i++)
    {
	printf("%5.2e  %5.2e\n", parts[i].xv, parts[i].yv);
    }
}

double * particleBox::findForces(double *nforces, double *xforces, double *yforces)
{
    double xdist, ydist, dist, invdist;
    
    for(int i=0; i<n; i++)
    {
	xforces[(i*n)+i] = 0.0;
	yforces[(i*n)+i] = 0.0;

	//do upper triangular b/c lower triangular is == *-1
	for(int j=i+1; j<n; j++)
	{
	    dist = minDist(i,j,&xdist,&ydist);
	    invdist = 1.0 / dist;
	    if(dist < EPS1) {
		//handle this ish
		fprintf(stderr, "Minimun distance breached in finding forces\n");
		fprintf(stderr, "Distance: %f, particles: %d & %d\n", dist, i,j);
	    } else if(dist < TRUNC){
		//whether this is pos or neg depends if we are
		// calculating for the field or the source point
		// field = no change, source = -F
		nforces[(i*n)+j] = -1.0*findForce(invdist);
		xforces[(i*n)+j] = nforces[(i*n)+j] * xdist / dist;
		yforces[(i*n)+j] = nforces[(i*n)+j] * ydist / dist;
		potentials[(i*n)+j] = findPot(invdist);
	    } else {
	        nforces[(i*n)+j] = 0.0;
		xforces[(i*n)+j] = 0.0;
		yforces[(i*n)+j] = 0.0;
	    }
	}
    }

    for(int i=0; i<n; i++)	//Fill lower triangle
    {
	for(int j=0; j<n; j++)
	{
                nforces[(j*n)+i] = -1.0*nforces[(i*n)+j];
		xforces[(j*n)+i] = -1.0*xforces[(i*n)+j];
		yforces[(j*n)+i] = -1.0*yforces[(i*n)+j];
	}
    }
    
    return nforces;
}

double particleBox::minDist(int i1, int i2, double * xdist, double *ydist)
{
    double xmin, ymin, min;
    double dist;
    double xfac, yfac;
    double oneL = (double)l;
    
    min = oneL;
    particle field;

    xfac = -oneL;
    for(int i=0; i<3; i++)
    {
	yfac = -oneL;
	for(int j=0; j<3; j++)
	{
	    field = move(parts[i2],xfac,yfac);
	    dist = findDist(parts[i1], field);
	    if(dist < min){
		min = dist;
		xmin = field.xp1 - parts[i1].xp1;
		ymin = field.yp1 - parts[i1].yp1;
	    }
	    yfac += oneL;
	}
	xfac += oneL;
    }

    *xdist = xmin;
    *ydist = ymin;

    return min;
}

double particleBox::findForce(double invr)
{
    double rn13 = pow(invr,13);	//r ** -13 (r to the negative 13)
    double rn7  = pow(invr,7);	//r ** -7
    double retval = CONST1*(2*SIG13*rn13 - SIG7*rn7);
    return retval;
}

double particleBox::findPot(double invr)
{
    double rn12 = pow(invr, 12);
    double rn6 = pow(invr, 6);
    double retval = CONST2*(SIG12*rn12 - SIG6*rn6);
    return retval;
}

double particleBox::findDist(particle p1, particle p2)
{
    double xdist = p1.xp1 - p2.xp1;
    double ydist = p1.yp1 - p2.yp1;
    double retval = sqrt(xdist*xdist + ydist*ydist);
    return retval;
}

particle particleBox::move(particle p, double x, double y)
{
    particle retp;
    retp.xp1 = p.xp1 + x;
    retp.xp0 = p.xp0 + x;
    retp.xv = p.xv;
    retp.yp1 = p.yp1 + y;
    retp.yp0 = p.yp0 + y;
    retp.yv = p.yv;
    return retp;
}


void particleBox::step()
{
    findForces(nforces, xforces, yforces);
    updateParts();
}

void particleBox::updateParts()
{
    double newx, newy;		//new coordinates
    double netx, nety;		//net forces
    double twoDelt = 2.0*delt;
    double oneL = (double)l;

    for(int i=0; i<n; i++)
    {
        netx = 0.0;
        nety = 0.0;
	for(int j=0; j<n; j++)
	{
	    if(j != i) {
		netx += xforces[(i*n)+j];
		nety += yforces[(i*n)+j];
	    }
	}
	newx = 2*parts[i].xp1 - parts[i].xp0 + netx*delt2;
	newy = 2*parts[i].yp1 - parts[i].yp0 + nety*delt2;

	//check if new position crossed a boundary
	if(newx < 0.0){
	    newx += oneL;
	    parts[i].xp0 += oneL;
	    parts[i].xp1 += oneL;
	    parts[i].xcrosses--;
	} else if(newx > oneL){
	    newx -= oneL;
	    parts[i].xp0 -= oneL;
	    parts[i].xp1 -= oneL;
	    parts[i].xcrosses++;
	}

        if(newy < 0.0){
	    newy += oneL;
	    parts[i].yp0 += oneL;
	    parts[i].yp1 += oneL;
	    parts[i].ycrosses--;
	} else if(newy > oneL){
	    newy -= oneL;
	    parts[i].yp0 -= oneL;
	    parts[i].yp1 -= oneL;
	    parts[i].ycrosses++;
	}
	
	//update velocities
	parts[i].xv = (newx - parts[i].xp0) / twoDelt;
	parts[i].yv = (newy - parts[i].yp0) / twoDelt;
	//update positions
	parts[i].xp0 = parts[i].xp1;
	parts[i].yp0 = parts[i].yp1;
	parts[i].xp1 = newx;
	parts[i].yp1 = newy;
    }
}


void particleBox::copy(particle src, particle *dst)
{
    dst->xp0 = src.xp0;
    dst->xp1 = src.xp1;
    dst->xv = src.xv;
    dst->xcrosses = src.xcrosses;

    dst->yp0 = src.yp0;
    dst->yp1 = src.yp1;
    dst->yv = src.yv;
    dst->ycrosses = src.xcrosses;
}

void particleBox::heatBox(double factor)
{
    for(int i=0; i<n; i++)
    {
	parts[i].xp0 = parts[i].xp1 - factor*(parts[i].xp1-parts[i].xp0);
        parts[i].yp0 = parts[i].yp1 - factor*(parts[i].yp1-parts[i].yp0);
    }
}

