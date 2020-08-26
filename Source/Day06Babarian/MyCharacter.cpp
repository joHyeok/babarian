// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	//�ϵ��ڵ�
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);

	PlayerInputComponent->BindAxis(TEXT("CameraView"), this, &AMyCharacter::CameraView);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMyCharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMyCharacter::StopSprint);
}

void AMyCharacter::MoveForward(float AxisValue)
{
	//���� ȸ��, ī�޶� ���� ����
	//GetControlRotation().Vector()
	FVector CameraLocation;
	FRotator CameraRotator;
	//��Ʈ�ѷ��� ī�޶��� ������ ������ �ִ�. ī�޶��� ��ġ, ȸ������ �����Ѵ�.
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotator);

	// ĳ������ �̵��� ī�޶��� �������� �̵���ų���� ��Ʈ�ѷ��� �������� �̵���ų����
	// ���ؾ��Ѵ�. ���������� ���� ȸ������ ���������� �� ������������ ���������μ�
	// �ٸ� ����� ���´�.

	//ī�޶��� �������� �̵��Ѵ�. ���������� yaw�������θ� �̵��Ѵ�.
	//FRotator YawBaseRotation = FRotator(0, CameraRotator.Yaw, 0);

	// ��Ʈ�ѷ��� �������� �̵��Ѵ�.
	FRotator YawBaseRotation = FRotator(0, GetControlRotation().Yaw, 0);

	FVector CameraForward = UKismetMathLibrary::GetForwardVector(YawBaseRotation);

	AddMovementInput(CameraForward, AxisValue);
}

void AMyCharacter::MoveRight(float AxisValue)
{
	FRotator YawBaseRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector CameraRight = UKismetMathLibrary::GetRightVector(YawBaseRotation);
	AddMovementInput(CameraRight, AxisValue);
}

void AMyCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AMyCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AMyCharacter::CameraView(float AxisValue)
{

}

void AMyCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AMyCharacter::StopSprint() {
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}