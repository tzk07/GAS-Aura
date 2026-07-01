// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	
	check(AttributeInfo);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		// // Pair 要按值捕获，而不是引用捕获。因为循环结束后引用可能指向错误内容或失效
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				AttributeInfoDelegate.Broadcast(Info);
			}	
		);
	}
}
