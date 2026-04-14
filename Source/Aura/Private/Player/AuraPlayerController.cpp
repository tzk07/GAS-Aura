// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Inreraction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	// 网络同步开关，开启后，服务器在当前玩家的数据发生变化时，需要同步给其他客户端
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	// TraceChannel: ECC_Visibility 追踪通道，只要游戏世界里的物体（比如地板、墙壁、怪物）设置了 “阻挡可视性（Block Visibility）”，射线就会被它拦住
	// bTraceComplex: false 使用简单碰撞检测，节省性能
	// CursorHit: 输出结果
	GetHitResultUnderCursor(ECC_Visibility, false , CursorHit);
	
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	/** 
	 * Line Trace from cursor. There are several scenarios:
	 *  A. LastActor == null && ThisActor == null
	 *		- Do nothing
	 *	B. LastActor == null && ThisActor != null (Valid)
	 *		- Highlight ThisActor
	 *	C. LastActor is Valid && ThisActor == null
	 *		- Unhighlight LastActor
	 *	D. Both Actors are Valid but ThisActor != LastActor
	 *		- Highlight ThisActor and Unhighlight LastActor
	 *	E. Both Actors are Valid and ThisActor == LastActor
	 *		- Do nothing
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B: Highlight ThisActor
			ThisActor->HighlightActor();
		} 
		else
		{
			// Case A: Do nothing
		}
	}
	else // LastActor is Valid
	{
		if (ThisActor == nullptr)
		{
			// Case C: Unhighlight LastActor
			LastActor->UnHighlightActor();
		} 
		else
		{
			if (ThisActor != LastActor)
			{
				// Case D: Highlight ThisActor and Unhighlight LastActor
				ThisActor->HighlightActor();
				LastActor->UnHighlightActor();
			}
			else
			{
				// Case E: Do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 检查 AuraContext 是否正确设置
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
	bShowMouseCursor = true; // 显示鼠标光标
	DefaultMouseCursor = EMouseCursor::Default; // 光标样式
	
	// 配置鼠标与输入模式
	FInputModeGameAndUI InputModeData; // 声明一个配置对象(配置信息)
	// 设置鼠标不要锁定在游戏视口 (Viewport) 内，允许鼠标移出游戏窗口
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 设置在鼠标被 "捕获" 时 (通常是按下鼠标按键拖拽时) 不要隐藏鼠标指针
	InputModeData.SetHideCursorDuringCapture(false);
	// 执行配置
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	// ETriggerEvent::Triggered: 按键按住时，每一帧都会调用函数 Move
	EnhancedInputComponent->BindAction(MoveACtion, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 拆箱：从 InputActionValue 中获取输入数据
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	// 获取控制器（也就是玩家/摄像机）当前的朝向
	const FRotator Rotation = GetControlRotation();
	// 抹平视线：过滤掉抬头和低头的角度
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	// FRotationMatrix(YawRotation)：基于刚才的水平旋转生成一个旋转矩阵
	// GetUnitAxis：从这个矩阵中提取出方向向量，X 为当前视角正前方
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	// 获取当前控制器附着的角色 Pawn
	if (APawn* ControllerPawn = GetPawn<APawn>())
	{
		// 沿 ForwardDirection 方向移动，力度(速度) 为 InputAxisVector.Y(W/S键的输入值)
		ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}


