// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * 
 * @param Target 效果的接收者
 * @param GameplayEffectClass 效果的创建模板 
 */
void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	/*
		IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
		if (ASCInterface)
		{
			 * 更快一点，确定目标实现了 GetAbilitySystemComponent() 接口
			 * ASCInterface->GetAbilitySystemComponent();
			 * 通过“搜索”访问。我不确定你在哪，但我会翻遍你的全身（接口、组件、关联类）把你找出来
			 * UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		}
	*/
	// 从 Target 上获取 ASC
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;
	
	check(GameplayEffectClass);
	
	// ContextHandle 轻量级句柄，存储 Context 的指针，记录了这次效果触发时的环境信息
	// 主要用于逻辑判定（比如：如果伤害来源是队友，则减免伤害）和表现（比如：在被命中的那个点播放粒子特效）
	// 可以自定义扩展
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	// 明确标记这个效果的 发出者
	EffectContextHandle.AddSourceObject(this);
	
	// EffectSpec 存储了经过计算后的最终指令数据
	// Spec 包含了 Context
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}


