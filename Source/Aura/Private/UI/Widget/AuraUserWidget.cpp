// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* inWidgetController)
{
	// 把控制器存进 WidgetController
	WidgetController = inWidgetController;
	// 调用 WidgetControllerSet() 通知蓝图
	WidgetControllerSet();
}
