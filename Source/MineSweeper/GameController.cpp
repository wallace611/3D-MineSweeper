// Fill out your copyright notice in the Description page of Project Settings.


#include "GameController.h"

// Sets default values
AGameController::AGameController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(PlayerRoot);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(PlayerRoot);

	// Set top camera
	TopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	TopCamera->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->TargetArmLength = 4000.0f;

	TopCamera->FieldOfView = 90.0f;

}

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();

	

	if (this->PlayerControllerRef)
	{
		this->PlayerControllerRef->bShowMouseCursor = 1;
		bCursorShow = 1;
	}
}

void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->CursorPointing();
	this->CursorAtCenter();
}

// Called to bind functionality to input
void AGameController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ToggleCursor", IE_Pressed, this, &AGameController::HideCursor);
	PlayerInputComponent->BindAction("ToggleCursor", IE_Released, this, &AGameController::ShowCursor);
	PlayerInputComponent->BindAction("StepOn", IE_Released, this, &AGameController::LeftClickOn);
	PlayerInputComponent->BindAction("ToggleFlag", IE_Pressed, this, &AGameController::MiddleClickOn);
	PlayerInputComponent->BindAxis("Turn", this, &AGameController::YawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AGameController::PitchInput);
	PlayerInputComponent->BindAxis("Zoom", this, &AGameController::Zoom);
}

void AGameController::YawInput(float val)
{
	if (!bCursorShow)
		APawn::AddControllerYawInput(val);
}

void AGameController::PitchInput(float val)
{
	if (!bCursorShow)
		APawn::AddControllerPitchInput(val);
}

void AGameController::Zoom(float val)
{
	if (SpringArmComponent->TargetArmLength - val * 100.0f > 200.0f)
		SpringArmComponent->TargetArmLength -= val * 100.0f;
}

void AGameController::MouseLineTracing(FHitResult& HitResult)
{
	FVector StartLocation;
	FVector EndDirection;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActors(SummonedActor);

	// Get mouse position and determine the start and end
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(StartLocation, EndDirection);
	EndDirection *= 100.0f * SpringArmComponent->TargetArmLength;
	EndDirection += StartLocation;

	// Executing the line trace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndDirection, ECC_Visibility, QueryParams);
}

void AGameController::LeftClickOn()
{
	if (bCursorShow && !bMenuOpened)
	{
		FHitResult SelectedArea;
		this->MouseLineTracing(SelectedArea);

		// Calculate row and column
		int32 Onrow = (int32)(SelectedArea.Location.X + (this->GameCore->GetRow() * 50)) / 100;
		int32 Oncolumn = (int32)(SelectedArea.Location.Y + (this->GameCore->GetColumn() * 50)) / 100;

		UE_LOG(LogTemp, Warning, TEXT("Step on row: %d column: %d"), Onrow, Oncolumn);
		if(SelectedArea.GetActor() == BoardRef)
			this->StepOn(Onrow, Oncolumn);
	}
}

void AGameController::MiddleClickOn()
{
	if (bCursorShow)
	{
		FHitResult SelectedArea;
		this->MouseLineTracing(SelectedArea);

		// Calculate row and column
		int32 Onrow = (int32)(SelectedArea.Location.X + (this->GameCore->GetRow() * 50)) / 100;
		int32 Oncolumn = (int32)(SelectedArea.Location.Y + (this->GameCore->GetColumn() * 50)) / 100;

		UE_LOG(LogTemp, Warning, TEXT("Set flag on row: %d column: %d"), Onrow, Oncolumn);
		if (SelectedArea.GetActor() == BoardRef)
			this->SetFlag(Onrow, Oncolumn);
	}
}

void AGameController::StartGame(const int32& Setrow, const int32& Setcolumn, const int32& SetBomb)
{
	if (Setrow * Setcolumn < 9)
		return;
	if (Setrow * Setcolumn - 9 < SetBomb)
		return;
	// Kill all actor
	this->DestoryActor();
	this->SummonedActor.Empty();
	this->IconOnBoardRef.Empty();
	this->FlagOnBoardRef.Empty();

	// Reset and generate the board
	this->GameCore->Reset();
	this->GameCore->SetSize(Setrow, Setcolumn);
	this->GameCore->SetBomb(SetBomb);
	this->GameCore->Generate();

	SetSpawnFlagSize();

	// Print board to Log
	MineSweeperActions::PrintBoard(this->GameCore);

	// Spawn the board mesh and cursor
	this->SpawnBoard();
	this->SpawnCursorOnBoard();
}

void AGameController::StepOn(const int32& Onrow, const int32& Oncolumn)
{
	if (GameCore->GameEnd != 0)
		return;
	TArray<TPair<int32, int32>> SteppedArea;
	switch (this->GameCore->StepOn(Onrow, Oncolumn, SteppedArea))
	{
	case 1:
		UE_LOG(LogTemp, Warning, TEXT("You lose"));
		this->GameCore->Reveal();
		this->Reveal();
		break;
	case 2:
		UE_LOG(LogTemp, Warning, TEXT("You win"));
		this->GameCore->Reveal();
		this->Reveal();
	default:
		break;
	}
	MineSweeperActions::PrintBoard(this->GameCore);
	if (GameCore->GameEnd == 0 || GameCore->GameEnd == 2)
	{
		for (TPair<int32, int32> Area : SteppedArea)
		{
			SpawnIcon(Area.Key, Area.Value);
		}
	}
}

void AGameController::SetFlag(const int32& Onrow, const int32& Oncolumn)
{
	if (GameCore->GameEnd != 0)
		return;
	if (this->GameCore->ToggleFlag(Onrow, Oncolumn) == 0)
	{
		ToggleSpawnFlag(Onrow, Oncolumn);
		MineSweeperActions::PrintBoard(this->GameCore);
	}
}

void AGameController::Reveal()
{
	for (auto Area : this->GameCore->BombLocation)
	{
		this->SpawnIcon(Area.Key, Area.Value);
	}
}


void AGameController::HideCursor()
{
	if (PlayerControllerRef)
	{
		PlayerControllerRef->bShowMouseCursor = 0;
		bCursorShow = 0;
		if (ACursorActor* CursorActorRef = Cast<ACursorActor>(CursorOnBoardRef))
		{
			CursorActorRef->HideCursor();
		}
	}
}

void AGameController::ShowCursor()
{
	if (PlayerControllerRef)
	{
		PlayerControllerRef->bShowMouseCursor = 1;
		bCursorShow = 1;
		if (ACursorActor* CursorActorRef = Cast<ACursorActor>(CursorOnBoardRef))
		{
			CursorActorRef->ShowCursor();
			FVector WorldCursorLocation = CursorActorRef->GetActorLocation();
			FVector2D ScreenLocation;
			PlayerControllerRef->ProjectWorldLocationToScreen(WorldCursorLocation, ScreenLocation);
			PlayerControllerRef->SetMouseLocation(ScreenLocation.X, ScreenLocation.Y);
		}
	}
}

void AGameController::CursorAtCenter()
{
	if (!bCursorShow)
	{
		int32 X;
		int32 Y;
		PlayerControllerRef->GetViewportSize(X, Y);
		PlayerControllerRef->SetMouseLocation(X / 2, Y / 2);
	}
}

void AGameController::CursorPointing()
{
	if (BoardRef)
	{
		if (bCursorShow)
		{
			FHitResult PointResult;
			this->MouseLineTracing(PointResult);
			if (PointResult.GetActor() == BoardRef)
			{
				int32 Onrow = (int32)(PointResult.Location.X + (this->GameCore->GetRow() * 50)) / 100;
				int32 Oncolumn = (int32)(PointResult.Location.Y + (this->GameCore->GetColumn() * 50)) / 100;

				// Call CursorActor only when the mouse has moved
				if ((Onrow != MouseLocation.Key || Oncolumn != MouseLocation.Value) && Onrow < GameCore->GetRow() && Oncolumn < GameCore->GetColumn())
				{
					ACursorActor* CursorActorRef = Cast<ACursorActor>(CursorOnBoardRef);
					FVector LocationToMove;
					LocationToMove.X = (float)Onrow * 100 - GameCore->GetRow() * 50 + 50;
					LocationToMove.Y = (float)Oncolumn * 100 - GameCore->GetColumn() * 50 + 50;
					LocationToMove.Z = 20.0f;
					CursorActorRef->MoveToLocation(LocationToMove);
					if (!CursorActorRef->bShowCursor)
					{
						CursorActorRef->ShowCursor();
					}
				}
				MouseLocation.Key = Onrow;
				MouseLocation.Value = Oncolumn;
			}
		}
	}
	
}

void AGameController::SetSpawnFlagSize()
{
	for (int i = 0; i < GameCore->GetRow(); i++)
	{
		TArray<AActor*> ATemp;
		ATemp.SetNum(GameCore->GetColumn());
		FlagOnBoardRef.Push(ATemp);
	}
}

void AGameController::SpawnBoard()
{
	if (this->Board)
	{
		BoardRef = GetWorld()->SpawnActor<AActor>(Board);
		UE_LOG(LogTemp, Warning, TEXT("Spawn board successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn board failed"));
	}
}

void AGameController::SpawnCursorOnBoard()
{
	if (this->CursorOnBoard)
	{
		CursorOnBoardRef = GetWorld()->SpawnActor<AActor>(CursorOnBoard);
		SummonedActor.Push(CursorOnBoardRef);
		UE_LOG(LogTemp, Warning, TEXT("Spawn Cursor successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Cursor failed"));
	}
}

void AGameController::SpawnIcon(const int32& Onrow, const int32& Oncolumn)
{
	FVector Location;
	Location.X = (float)Onrow * 100 - GameCore->GetRow() * 50 + 50;
	Location.Y = (float)Oncolumn * 100 - GameCore->GetColumn() * 50 + 50;
	Location.Z = 50.0f;

	AActor* IconActorRefTemp = GetWorld()->SpawnActor<AActor>(IconOnBoard, Location, FRotator(0.0f, 0.0f, 0.0f));
	IconOnBoardRef.Push(IconActorRefTemp);
	SummonedActor.Push(IconActorRefTemp);

	if (AIconActor* IconActorRef = Cast<AIconActor>(IconActorRefTemp))
	{
		IconActorRef->ShowIcon(this->GameCore->Board.Board[Onrow][Oncolumn]);
	}
}

void AGameController::ToggleSpawnFlag(const int32& Onrow, const int32& Oncolumn)
{
	if (FlagOnBoardRef[Onrow][Oncolumn])
	{
		if (AFlagActor* FlagActorRef = Cast<AFlagActor>(FlagOnBoardRef[Onrow][Oncolumn]))
		{
			FlagActorRef->Destroy();
			FlagOnBoardRef[Onrow][Oncolumn] = nullptr;
		}
	}
	else
	{
		FVector Location;
		Location.X = (float)Onrow * 100 - GameCore->GetRow() * 50 + 50;
		Location.Y = (float)Oncolumn * 100 - GameCore->GetColumn() * 50 + 50;
		Location.Z = 50.0f;
		FlagOnBoardRef[Onrow][Oncolumn] = GetWorld()->SpawnActor<AActor>(FlagOnBoard, Location, FRotator(0.0f, 0.0f, 0.0f));
		if (AFlagActor* FlagActorRef = Cast<AFlagActor>(FlagOnBoardRef[Onrow][Oncolumn]))
		{
			SummonedActor.Push(FlagOnBoardRef[Onrow][Oncolumn]);
		}
	}
}

void AGameController::DestoryActor()
{
	// Destory all actor
	UE_LOG(LogActor, Warning, TEXT("Delete %d actor(s)"), SummonedActor.Num());
	for (auto ActorRef : SummonedActor)
	{
		ActorRef->Destroy();
	}
	if(BoardRef)
		BoardRef->Destroy();
}
