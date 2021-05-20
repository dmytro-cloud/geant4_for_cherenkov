//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction, double energy_, double length_)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),
  energy(energy_),
  length(length_)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{

  std::string outputFileName = "Photons_distribution_" + std::to_string(length) + "_mkm_" + std::to_string(energy) + "_MeV.txt";

  fFile.open(outputFileName, std::ios::app);
  G4StepPoint* preStep = step->GetPostStepPoint();
  auto coordinate = preStep->GetPosition();
  auto momentum = preStep->GetMomentum();
  double electronEnergy = preStep->GetKineticEnergy();

  G4Track* track = step->GetTrack();
  auto particleName = track->GetParticleDefinition()->GetParticleName();
  // std::cout <<  coordinate.mag() << std::endl;
  // auto stepNumber = track->GetCurruntStepNumber();
  // std::cout << "WTF" << std::endl;
  eventNum = fEventAction->eventNum;
  if (particleName == "opticalphoton"  && coordinate.mag() > 900) {
    fFile << eventNum << '\t' << coordinate[0] << '\t' << coordinate[1] << '\t' << coordinate[2] << '\t' << electronEnergy << '\n';
  }
  fFile.close();
  // fFile.open("Trajectory_coordinates.txt", std::ios::app);
  // G4StepPoint* preStep = step->GetPreStepPoint();
  // auto coordinate = preStep->GetPosition();
  // auto momentum = preStep->GetMomentum();
  // double electronEnergy = preStep->GetKineticEnergy();

  // G4Track* track = step->GetTrack();
  // int track_id = track->GetTrackID();
  // auto particleName = track->GetParticleDefinition()->GetParticleName();
  // // auto stepNumber = track->GetCurruntStepNumber();
  // // std::cout << "WTF" << std::endl;
  // eventNum = fEventAction->eventNum;
  // if (particleName == "e-"){
  //   fFile << (eventNum + 1) << track_id <<'\t' << coordinate[0] << '\t' << coordinate[1] << '\t' << coordinate[2] << '\t'
  //        << momentum.mag() << '\n';
  // }
  // fFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

