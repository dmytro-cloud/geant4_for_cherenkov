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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction(double track_distance_)
: G4VUserDetectorConstruction(),
  fScoringVolume(0),
  track_distance(track_distance_)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 10*cm, env_sizeZ = track_distance * 1e-6; //mkm
  G4double envR = 10 * m;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

  // ------------ Generate & Add Material Properties Table ------------//
  G4double photonEnergy[] = {200.0, 220.0, 240.0, 260.0, 280.0, 300.0, 320.0,
     340.0, 360.0, 380.0, 400.0, 420.0, 440.0, 460.0, 480.0, 500.0, 520.0,
      540.0, 560.0, 580.0, 600.0, 620.0, 640.0, 660.0, 680.0, 700.0, 720.0,
       740.0, 760.0, 780.0, 800.0};

  const G4int nEntries = sizeof (photonEnergy) / sizeof (G4double);
  for (int i = 0; i < nEntries; i++){
    photonEnergy[i] = 200.0 / photonEnergy[i] * 0.030996000000000003 * eV * 200.0; // To eV
    std::cout << photonEnergy[i] << std::endl;
  }

  G4double refractiveIndex[] = {1.41615, 1.39727, 1.38395, 1.37414, 1.36667, 1.36082, 1.35615,
   1.35233, 1.34916, 1.3465, 1.34423, 1.34227, 1.34058, 1.33909, 1.33778, 1.33661, 1.33557, 1.33463,
   1.33378, 1.33301, 1.33231, 1.33167, 1.33108, 1.33054, 1.33004,
   1.32957, 1.32914, 1.32874, 1.32836, 1.32801, 1.32768};

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();
  myMPT1->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries) ->SetSpline(true);
  env_mat->SetMaterialPropertiesTable(myMPT1);
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  // G4double world_sizeXY = 1*env_sizeXY;
  // G4double world_sizeZ  = 1*env_sizeZ;
  G4double worldR = 1 * envR;

  // Instead of vacuum
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  // G4Box* solidWorld =    
  //   new G4Box("World",                       //its name
  //      0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
  G4Sphere* solidWorld = new G4Sphere("World", 0, worldR, 0, 2 * 5.0, 0, 5.0);

  G4VisAttributes * worldAtt = new G4VisAttributes(G4Colour(255.,255.,255., 1));
  // worldAtt->SetForceWireframe(true);

      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  logicWorld->SetVisAttributes(worldAtt);

  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
  // G4Sphere* solidEnv = new G4Sphere("World", 0, envR, 0, 2 * 5.0, 0, 5.0);
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 

  fScoringVolume = logicEnv;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
