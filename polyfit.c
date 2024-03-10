/*
	Constructs the descrete weighted least squares approximation by polynomials of
	degree less than nterms to given data.

	Inputs;
		x[i], f[i]   i=0 .. npoint-1 gives the abscissae and ordinates of the given data
			to be fit
		w npoint vector containing the positive weights to be used
		npoint number of data points.
		nterms the order (degree + 1) of the polynomial approximant ( <= 20)

	Work areas;
		pjm1, pj arrayes of length npoint to containe the values at the x's of the two most
		recent orthogonal polynomials.

	Outputs;
		error npoint vector containing the error at the x's of the polynominal approximant to
		the given data.

	Output via externals;
		b,c arrayes contining the coefficients for the three-term recurrence which generates
		the orthogonal polynomials.
		d coefficients of the polynomial approximant to the given data with respect to the
		sequence of orthogonal polynomials.
		The value of the approximant at a point y may be obtained by a reference to ortval(y).

	Method;
		The sequence p0, p1,...,pnterms-1 of orthogonal polynomials with respect to the discrete
		inner product
			(p,q) = sum(p(x[i])*q(x[i])*w[i], i=0,....npoint-1
		is generated in terms of their three-term recurrence
			pjp1(x)=(x-b[j+1])*pj(x)-c[j+1]*pjm1(x)
		and the coefficient d[j] of the weighted least squares approximation to the given data
		is obtained concurrently as
			d[j+1] = (f,pj)/(pj,pj)  j=0,.....nterms-1
		actually, in order to reduce cancellation, (f,pj) is calculated as (error,pj) with 
		error=f initially, and for each j, error reduced by d[j+1]*pj as soon as d[j+1]
		becomes available.
*/

#include "config.h"
#include "polyfit.h"

static double b[20],c[20],d[20];   /* external variables used by ortval */
static int numterm;

void ortpol(double x[], double f[], double w[], int npoint, double pjm1[], double pj[], double error[], int nterms)

//void ortpol(x,f,w,npoint,pjm1,pj,error,nterms)
	//double x[],f[],pj[],pjm1[],w[],error[];
	//int npoint,nterms;
{
	int i,j;
	double p,s[20];

	numterm=nterms;

	for(j=0;j<nterms;j++)
		b[j]=d[j]=s[j]=0.0;
	c[0]=0.0;
	for(i=0;i<npoint;i++)
	{
		d[0]+=(f[i]*w[i]);
		b[0]+=(x[i]*w[i]);
		s[0]+=w[i];
	}
	d[0]/=s[0];
	for(i=0;i<npoint;i++)
		error[i]=f[i]-d[0];
	if(nterms == 1) return;
	b[0]/=s[0];
	for(i=0;i<npoint;i++)
	{
		pjm1[i]=1.0;
		pj[i]=x[i]-b[0];
	}

	for(j=1; j<nterms;j++)
	{
		for(i=0;i<npoint;i++)
		{
			p=pj[i]*w[i];
			d[j]+=(error[i]*p);
			p*=pj[i];
			b[j]+=(x[i]*p);
			s[j]+=p;
		}
		d[j]/=s[j];
		for(i=0;i<npoint;i++)
			error[i]-=(d[j]*pj[i]);
		if(j == nterms-1) return;
		b[j]/=s[j];
		c[j]=s[j]/s[j-1];
		for(i=0;i<npoint;i++)
		{
			p=pj[i];
			pj[i]=(x[i]-b[j])*pj[i]-c[j]*pjm1[i];
			pjm1[i]=p;
		}
	}
}

/*
	Returns the value at x of the polynomial of degree less than nterms given by
		d[1]*po(x)+d[2]*p1(x)+.....+d(nterms)*pnterms-1(x)
	with the sequence p0,p1,... of orthogonal polynomials generated by the three term
	recurrence
		pjp1(x)=(x-b[j+1])*pj(x)-c[j+1]*pjm1(x), all j
*/

double ortval(double x)
// double ortval(x)
//	double x;
{
	extern double b[20],c[20],d[20];
	extern int numterm;
	double prev,yval,prev2;
	int k;

	prev=0.0;
	yval=d[numterm-1];
	if(numterm == 1) return(yval);
	for(k=numterm-2;k>=0;k--)
	{
		prev2=prev;
		prev=yval;
		yval=d[k]+(x-b[k])*prev-c[k+1]*prev2;
	}
	return(yval);
}
