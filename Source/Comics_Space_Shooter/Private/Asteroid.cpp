// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

#include "GameModeSpace.h"
#include "Projectile.h"
#include "SpaceShip.h"
#include "Components/ProgressBar.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameMode.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Ajoute un SphereComponent pour la détection des collisions
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->InitSphereRadius(50.0f); // Tu peux ajuster la taille du rayon de la sphère
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Utilisation de QueryOnly pour ne pas avoir de simulation physique
	CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); 
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // Répond aux collisions avec le vaisseau

	// Bind de l'événement de collision
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnOverlapBegin);
	
	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetupAttachment(Mesh);

	//Positionner au dessus du mesh
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 250.f)); // Ajuste Z selon ton mesh
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen); // Toujours face à la caméra
	HealthWidget->SetDrawSize(FVector2D(100, 100));

	if (HealthWidget)
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Comics_Space_Shooter/UI/BP_AsteroidHealth"));
		if (WidgetClass.Succeeded())
		{
			HealthWidget->SetWidgetClass(WidgetClass.Class);
		}
	}
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();

	// Choisir un mesh aléatoire
	if(MeshVariants.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, MeshVariants.Num() - 1);

		if (Index == 2)
		{
			CollisionSphere->SetSphereRadius(20250.0f);
		}
		else
		{
			CollisionSphere->SetSphereRadius(6250.0f);
		}
		
		Mesh->SetStaticMesh(MeshVariants[Index]);
	}

	Life = FMath::RandRange(3, 10);
	MaxLife = Life;
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		// On ignore la composante Z
		Direction.Z = 0.f;
		Direction.Normalize(); // re-normaliser après modification

		FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;

		// Garder Z fixe (ex: 1000)
		NewLocation.Z = 1000.f;

		SetActorLocation(NewLocation);
		
	}

}

void AAsteroid::UpdateHealthBar()
{
	if (!HealthWidget) return;

	UUserWidget* Widget = HealthWidget->GetUserWidgetObject();
	if (!Widget) return;

	UProgressBar* HealthBar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("Health"))); // Nom du ProgressBar
	if (HealthBar)
	{
		HealthBar->SetPercent(Life / MaxLife);
	}
}


void AAsteroid::DestroyAsteroid()
{
	Life -= 1;

	if (AGameModeSpace* GM = Cast<AGameModeSpace>(GetWorld()->GetAuthGameMode()))
	{
		GM->AddScore(10);
	}

	UpdateHealthBar();
	
	if (Life <= 0)
	{
		UWorld* World = GetWorld();
		if (ExplosionClass && World)
		{
			FRotator Rotation = FRotator(0.f, 90.f, 0.f);
			FVector Location = GetActorLocation();
			Location.Z = 1000.f;

			World->SpawnActor<AActor>(
				ExplosionClass,
				Location,
				Rotation
			);
		}

		Destroy();
	}
}

void AAsteroid::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
								bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ASpaceShip::StaticClass()))
	{
		if (AGameModeSpace* GM = Cast<AGameModeSpace>(GetWorld()->GetAuthGameMode()))
		{
			GM->RemoveLife();  // Retirer une vie du vaisseau
		}

		// Détruire l'astéroïde après la collision
		Destroy();
	}
}