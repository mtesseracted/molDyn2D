#include<stdio.h>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>

#define DIGITS 6

//basis gnuplot string
//p 'Pos.dat' every ::1::1 u 1:2 ps 3 pt 6, 'Pos.dat' every ::1::1000 u 1:2 w lp 1

//convert n to a string of digits long in base 10
//return empty if over value allowed
std::string myitoa(int n, int digits)
{
   int max = pow(10,digits);
   if( n >= max ) return std::string("");
   std::string ret;
   max /= 10;
   for(int i=0; i<digits; i++){
      ret += ((n / max) % 10)+48;
      max /= 10;
   }
   return ret;
}

int main(int argc, char **argv)
{
    if(argc < 4){
	printf("Usage:>%s <side_length> <particles> <steps>\n", argv[0]);
	exit(1);
    }
    int p = strtol(argv[2],NULL,10);
    int length = strtol(argv[1],NULL,10);
    int steps = strtol(argv[3],NULL,10);
    int trailLen = 50;
    int skipFrames = 1;

    std::string image = "\"pos";
    std::string extension = ".png\"";
    std::string outnum = myitoa(0,DIGITS);
    std::string outfile = image;
    outfile += outnum;
    outfile += extension;

    char str1[120] = "'Pos.dat' every ::%d::%d u %d:%d ps 3 pt 6 lt -1, 'Pos.dat' every ::%d::%d u %d:%d pt 6 ps 0.4";
    char str2[120];
    char str3[120];

    printf("set term pngcairo\n");
    printf("unset key\n");
    printf("set xrange [0:%d]\n",length);
    printf("set yrange [0:%d]\n",length);

    int trail = 0;
    for(int j=9; j<steps; j+=skipFrames)
    {
       printf("set output %s\n", outfile.c_str());
       sprintf(str2,str1, j,j,1,2,j-trail,j,1,2);
       printf("p %s", str2);
       str2[0] = '\0';
       strcat(str2, ", ");
       strcat(str2, str1);
       strcpy(str3, str2);

       for(int i=3; i<2*p; i+=2)
       {
	   sprintf(str2, str3, j,j,i,i+1,j-trail,j,i,i+1);
   	   printf("%s", str2);
       }
       printf("\nreplot\n");
       if(trail < trailLen) trail++;
       outfile = image;
       outfile += myitoa(j,DIGITS);
       outfile += extension;
    }

    return 0;
}

