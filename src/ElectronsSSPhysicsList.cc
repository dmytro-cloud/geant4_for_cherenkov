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
/// \file GeantinoPhysicsList.cc
/// \brief Implementation of the GeantinoPhysicsList class

#include "ElectronsSSPhysicsList.hh"
#include "G4CoulombScattering.hh"
#include "G4Electron.hh"
#include "G4ParticleDefinition.hh"
#include "G4LeptonConstructor.hh"
#include "G4BosonConstructor.hh"

#include "G4PhysicsListHelper.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"

#include "G4BuilderType.hh"
#include "G4EmModelActivator.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4UrbanMscModel.hh"
#include "G4LossTableManager.hh"
#include "G4Cerenkov.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ElectronsSSPhysicsList::ElectronsSSPhysicsList()
  : G4VUserPhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ElectronsSSPhysicsList::~ElectronsSSPhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ElectronsSSPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  // G4Electron::ElectronDefinition();
  G4LeptonConstructor lConstructor;
  lConstructor.ConstructParticle();

  G4BosonConstructor bConstructor;
  bConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ElectronsSSPhysicsList::ConstructProcess()
{
  // Define transportation process

  AddTransportation();

  // // Cherenkov process
  G4Cerenkov* theCerenkovProcess = new G4Cerenkov("Cerenkov");
  // theCerenkovProcess-> SetTrackSecondariesFirst(true);
  G4int MaxNumPhotons = 300;
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);

    // high energy limit for e+- scattering models
  G4double highEnergyLimit = G4EmParameters::Instance()->MscEnergyLimit();

  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
   // Add standard EM Processes
  G4ParticleTable* table = G4ParticleTable::GetParticleTable();
  for(const auto& particleName : partList.PartNames()) {
    G4ParticleDefinition* particle = table->FindParticle(particleName);
    if (!particle) { continue; }

    if (particleName == "e-") {


      G4CoulombScattering* ss = new G4CoulombScattering();
      // if(G4EmParameters::Instance()->UseMottCorrection()) {
      //   ss->SetEmModel(new G4eSingleCoulombScatteringModel());
      // }

      ph->RegisterProcess(new G4eIonisation(), particle);
      ph->RegisterProcess(new G4eBremsstrahlung(), particle);
      ph->RegisterProcess(ss, particle);
      ph->RegisterProcess(theCerenkovProcess, particle);
      
    }

  }
  // G4ProcessManager *elManager = G4Electron::ElectronDefinition()->GetProcessManager();
  // elManager->AddProcess(new G4eSingleCoulombScatteringModel());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
