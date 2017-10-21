#include <iostream>

#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TH1F.h>

#include "MediumMagboltz.hh"
#include "SolidBox.hh"
#include "GeometrySimple.hh"
#include "ComponentConstant.hh"
#include "Sensor.hh"
#include "TrackHeed.hh"
#include "Plotting.hh"
#include "Random.hh"

using namespace Garfield;

double GetW(MediumMagboltz* gas);

int main(int argc, char * argv[])
{
	// Make a medium
	MediumMagboltz* gas = new MediumMagboltz();
	gas->SetTemperature(293.15);
	gas->SetPressure(760.);

	std::string g1 = "he";
	std::string g2 = "isobutane";
	double c1 = 0.;
	double c2 = 0.;
	for(c1 = 100.; c1 > 59.; c1 -= 5.) {
		c2 = 100. - c1;
		gas->SetComposition(g1, c1, g2, c2);
		double w = GetW(gas);

		std::cout << "=================="<< std::endl;
		std::cout << gas->GetName() << " " << c1 << "/" << c2 << std::endl;
		std::cout << "W = " << w << std::endl;
		std::cout << "=================="<< std::endl;
	}

	return 0;
}

double GetW(MediumMagboltz* gas)
{
	// Detector geometry
	// Gap [cm]
	const double width = 1.;
	SolidBox* box = new SolidBox(width / 2., 0., 0., width / 2., 10., 10.);
	GeometrySimple* geo = new GeometrySimple();
	geo->AddSolid(box, gas);

	// Make a component
	ComponentConstant* comp = new ComponentConstant();
	comp->SetGeometry(geo);
	comp->SetElectricField(100., 0., 0.);

	// Make a sensor
	Sensor* sensor = new Sensor();
	sensor->AddComponent(comp);

	// Track class
	TrackHeed* track = new TrackHeed();
	track->SetSensor(sensor);
	track->SetParticle("e-");
	track->SetMomentum(3.e6);

//	track->EnableDebugging();
	// Initial position and direction
	double x0 = 0., y0 = 0., z0 = 0., t0 = 0., dx0 = 1., dy0 = 0., dz0 = 0.;
	track->NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);

	return track->GetW();
}


