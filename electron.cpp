#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ConfigFile.hpp"

#define FILE_WRITE_INTERVAL 10

using namespace std;

/*
 * <x>(t), <N>(t), e(omega), ro(k)
 *
*/

int main(int argc, char* argv[])
{
	int 
		i,
		n,
		nPoints,
		counter;
	double
		t,
		tau,
		tau_end,		
		omega,
		k,
		x,
		rok,
		*psiR,
		*psiR2,		// psiR(tau + dtau/2)
		*psiI,
		*HR,
		*HR2,
		*HI,
		meanN,
		meanX,
		meanE;
	
/*	if(argc < 2)
	{
		cout << "No parameters file found!" << endl;
		return 1;
	}

	ConfigFile parameters(argv[1]);
	
	*/
	ConfigFile parameters("settings.in");

	nPoints = parameters.getValueOfKey<int>("N");
	n = parameters.getValueOfKey<int>("n");
	tau = parameters.getValueOfKey<double>("tau");
	tau_end = parameters.getValueOfKey<double>("tau_end");
	k = parameters.getValueOfKey<double>("k");
	omega = parameters.getValueOfKey<double>("omega")*M_PI*M_PI/2;
	
	cout << "Parameters:" << endl;
	cout << "N =\t\t" << nPoints << endl;
	cout << "n =\t\t" << n << endl;
	cout << "tau =\t\t" << tau << endl;
	cout << "tau_end =\t" << tau_end << endl;
	cout << "k =\t\t" << k << endl;
	cout << "omega =\t\t" << omega << endl;
	
	psiR = new double[nPoints+1];
	psiR2 = new double[nPoints+1];
	psiI = new double[nPoints+1];
	HR = new double[nPoints+1];
	HR2 = new double[nPoints+1];
	HI = new double[nPoints+1];
	
	fstream probabilityFile("probability.dat", fstream::out);
	fstream NFile("N.dat", fstream::out);
	fstream XFile("X.dat", fstream::out);
	fstream EFile("E.dat", fstream::out);
	
	for(i = 0; i <= nPoints; ++i)
	{
		x = 1./nPoints*i;
		psiR[i] = sqrt(2)*sin(n*M_PI*x);
		psiI[i] = 0;
		HI[i] = 0;
	}
	
	HR[0] = 0;
	HR2[0] = 0;
	HI[0] = 0;
	HR[nPoints] = 0;
	HR2[nPoints] = 0;
	HI[nPoints] = 0;

	for(t = 0, counter = 0; t < tau_end; t += tau, ++counter)
	{
		meanN = 0;
		meanX = 0;
		meanE = 0;

		if(counter%FILE_WRITE_INTERVAL == 0)
			probabilityFile << t << "\t";

		for(i = 0; i <= nPoints; ++i)
		{
			rok = psiR[i]*psiR[i] + psiI[i]*psiI[i];
			meanN += rok;
		}
		meanN /= nPoints;

		for(i = 0; i <= nPoints; ++i)
		{
			// psiR[i] /= sqrt(meanN);
			// psiI[i] /= sqrt(meanN);

			x = 1./nPoints*i;
			rok = psiR[i]*psiR[i] + psiI[i]*psiI[i];
			if(counter%FILE_WRITE_INTERVAL == 0)
				probabilityFile << rok << "\t";
			
			meanX += x*rok;
			meanE += (psiR[i]*HR[i] + psiI[i]*HI[i]);
		}
		meanX /= nPoints;
		meanE /= nPoints;

		if(counter%FILE_WRITE_INTERVAL == 0)
		{
			probabilityFile << endl;
			NFile << t << "\t" << meanN << endl;
			XFile << t << "\t" << meanX << endl;
			EFile << t << "\t" << meanE << endl;
		}

 		for(i = 0; i <= nPoints; ++i)
		{	
			//32
			psiR2[i] = psiR[i]+HI[i]*tau/2;
		}
		for(i = 1; i < nPoints; ++i)
		{
			x = 1./nPoints*i;		
			HR2[i] = -0.5*(psiR2[i+1]+psiR2[i-1]-2*psiR2[i])*nPoints*nPoints+k*(x-0.5)*psiR2[i]*sin(omega*(t+tau/2));
		}
		for(i = 0; i <= nPoints; ++i)
		{
			//33
			psiI[i] -= HR2[i]*tau;
		}
		for(i = 1; i < nPoints; ++i)
		{
			x = 1./nPoints*i;
			HI[i] = -0.5*(psiI[i+1]+psiI[i-1]-2*psiI[i])*nPoints*nPoints+k*(x-0.5)*psiI[i]*sin(omega*(t+tau));
			HR[i] = -0.5*(psiR[i+1]+psiR[i-1]-2*psiR[i])*nPoints*nPoints+k*(x-0.5)*psiR[i]*sin(omega*(t+tau));
		}
		for(i = 0; i <= nPoints; ++i)
		{
			//34
			psiR[i] = psiR2[i] + HI[i]*tau/2;
		}
		if(counter % 5000 == 0)
			cout << counter/1000 << "k / " << floor(tau_end/tau)/1000 << "k" << endl;
	}

	probabilityFile.close();
	NFile.close();
	XFile.close();
	EFile.close();

	return 0;
}
