// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "GameFramework/Actor.h"

#include "Traffic/RoutePlanner.h"
#include "Vehicle/VehicleSpawnPoint.h"

#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/DataTable.h"

#include "CoreMinimal.h"


#include "OpenDriveActor.generated.h"

USTRUCT(Blueprintable)
struct FSplineCompData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	unsigned int uid = 0u;

	UPROPERTY(EditAnywhere)
	TArray<FVector> splinePoints;
	UPROPERTY(EditAnywhere)
	TArray<FVector> splineTangents;
};


USTRUCT(Blueprintable)
struct FRoutePlannerData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		unsigned int uid = 0u;
	UPROPERTY(EditAnywhere)
		TArray<FSplineCompData> splineComp;
};


USTRUCT(Blueprintable)
struct FTrafficNavData : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
		TArray<FRoutePlannerData> routePlanners;

};


UCLASS()
class CARLA_API AOpenDriveActor : public AActor
{
  GENERATED_BODY()

protected:

  /// A UBillboardComponent to hold Icon sprite
  UBillboardComponent *SpriteComponent;

  /// Sprite for the Billboard Component
  UTexture2D *SpriteTexture;

public:

	UPROPERTY(Category = "Generate", EditAnywhere)
	FString SaveDirectory = FString("C:/Users/KETI/Documents/carla2/carla/Unreal/CarlaUE4");
	UPROPERTY(Category = "Generate", EditAnywhere)
	FString FileName = FString("routes.csv");
private:
	FString TextToSave = FString("");
	FString AbsoluteFilePath = SaveDirectory + "/" + FileName;
  UPROPERTY()
  TArray<ARoutePlanner *> RoutePlanners;

  UPROPERTY()
  TArray<AVehicleSpawnPoint *> VehicleSpawners;

#if WITH_EDITORONLY_DATA
  /// Generate the road network using an OpenDrive file (named as the current
  /// .umap)
  UPROPERTY(Category = "Generate", EditAnywhere)
  bool bGenerateRoutes = false;
#endif // WITH_EDITORONLY_DATA

  // UPROPERTY(Category = "Generate", EditAnywhere)
  // FTrafficNavData NaM;

  /// Distance between waypoints where the cars will drive
  UPROPERTY(Category = "Generate", EditAnywhere, meta = (ClampMin = "0.01", UIMin = "0.01"))
  float RoadAccuracy = 2.f;

  /// Trigger elevantion
  UPROPERTY(Category = "Generate", EditAnywhere, meta = (ClampMin = "0.01", UIMin = "0.01"))
  float TriggersHeight = 100.f;

#if WITH_EDITORONLY_DATA
  /// Remove the previously generated road network. Also, it will remove
  /// spawners if necessary
  UPROPERTY(Category = "Generate", EditAnywhere)
  bool bRemoveRoutes = false;
#endif // WITH_EDITORONLY_DATA

  /// If true, spawners will be placed when generating the routes
  UPROPERTY(Category = "Spawners", EditAnywhere)
  bool bAddSpawners = false;

  /// If true, spawners will be placed on junctions too
  UPROPERTY(Category = "Spawners", EditAnywhere)
  bool bOnIntersections = false;

  /// Determine the height where the spawners will be placed, relative to each
  /// RoutePlanner
  UPROPERTY(Category = "Spawners", EditAnywhere)
  float SpawnersHeight = 300.f;

#if WITH_EDITORONLY_DATA
  /// Remove already placed spawners if necessary
  UPROPERTY(Category = "Spawners", EditAnywhere)
  bool bRemoveCurrentSpawners = false;
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITORONLY_DATA
  /// Show / Hide additional debug information
  UPROPERTY(Category = "Debug", EditAnywhere)
  bool bShowDebug = true;
#endif // WITH_EDITORONLY_DATA

public:

  AOpenDriveActor(const FObjectInitializer &ObjectInitializer);

  void ExportNavData();

  void BuildRoutes();

  void BuildRoutes(FString MapName);

  /// Remove all the existing ARoutePlanner and VehicleSpawners previously
  /// generated by this class to avoid overlapping
  void RemoveRoutes();

  void DebugRoutes() const;

  void RemoveDebugRoutes() const;

#if WITH_EDITOR
  void PostEditChangeProperty(struct FPropertyChangedEvent &PropertyChangedEvent);

#endif // WITH_EDITOR

  void AddSpawners();

  void RemoveSpawners();

  void BeginPlay();
};
