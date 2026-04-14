// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 蓝图可调用函数：外部可以给这个UI设置一个 Controller
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* inWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
protected:
	// 蓝图可实现事件：C++触发，蓝图里写逻辑
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
