#include "config.h"
#include "global.h"
#include "func.h"
#include <math.h>

#define SRTWO 1.414213562

double power1(double x,double A,double B)	/* y = pow(x,A) + B */
//double x,A,B;
{
	double value;
	value = pow(x,A) + B ;
	return(value);
}

double power2(double x,double A,double B,double C)	/* y = A *pow(x,B) + C */
//double x,A,B,C;
{
	double value;
	value = A * pow(x,B) + C ;
	return(value);
}

double normal(double x,double mean,double sigma)
//double x,mean,sigma;
{
	double value, temp;
	temp = pow((x-mean)/(sigma),2.0) / 2.0 ;
	value = exp(-temp) / sqrt(TWOPI) / sigma ;
	return(value);
}

double chisqr(double x,double ndf)
//double x, ndf;
{
	double value, temp, gam;
        extern int signgam;
        gam = tgamma(ndf/2.0);
	gam = exp(gam);
	temp = pow(2.0,ndf/2.0) * gam ;
	value = pow(x,(ndf-2.0)/2.0) * exp(-x/2.0) / temp ;
	return(value);
}

double scchisqr(double x,double sigma,double ndf,double offset)
//double x, sigma, ndf, offset;
{
	double value, temp, gam, sigx;

        sigx = sigma/(sqrt(ndf)*SRTWO); sigx = sqrt(sigx);
        gam = tgamma(ndf/2.0); gam = exp(gam);
	temp = pow(sigx,ndf) * pow(2.0,ndf/2.0) * gam ;
	if(offset>=x) value = 0.0;
	else value = pow((x-offset),(ndf-2.0)/2.0) * exp((offset-x)/(2.0*sigx*sigx)) / temp ;
	return(value);
}

double rclow(double x,double A,double B)		/* 	y = [1/(x*B)]*sqrt[1+x*x*B*B*A*A]	*/
//double x,A,B;
{
	double value;
	value = (1.0/(x*B))*sqrt((1.0+pow((x*B*A),2.0)));
	return(value);
}

double rcph(double x,double A,double B)		/* 	y = -1.0 * atan((1.0/(x*A*B))	*/
//double x,A,B;
{
	double value;
	value = -1.0 * atan((1.0/(x*A*B)));
	return(value);
}

double genexp(double x,double A,double B,double C,double D)		/* 	y = A * exp(B*x+C) + D 	*/
//double x,A,B,C,D;
{
	double value;
	value = A * exp(B*x+C) + D;
	return(value);
}

double gensin(double x,double A,double B,double C,double D)		/* 	y = A * sin(B*x+C) + D	*/
//double x,A,B,C,D;
{
	double value;
	value = A * sin(B*x+C) + D;
	return(value);
}

double ExpLin(double x,double A,double B,double C)		/* 	y = A * (1.0 - exp(B*x)) + C*x	*/
//double x,A,B,C;
{
	double value;
	value = A * (1.0 - exp(B*x)) + C * x;
	return(value);
}

double Poly4(double x,double A,double B,double C,double D,double E)	/* y = A + B*x + C*pow(x,2) + D*pow(x,3) + E*pow(x,4) */
//double x,A,B,C,D,E;
{
	double value;
	value = A + B*x + C*pow(x,2.0) + D*pow(x,3.0) + E*pow(x,4.0);
	return value;
}
