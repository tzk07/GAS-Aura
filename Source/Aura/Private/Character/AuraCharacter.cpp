// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// 允许角色自动转身，使其面朝移动的方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 设置转身的速度
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true; // 限制在平面上
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// 告诉当前的 Character，绝对不要跟随 Controller 的旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
