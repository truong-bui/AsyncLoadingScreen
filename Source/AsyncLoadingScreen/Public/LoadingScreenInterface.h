/**********************************************************************************
 *
 * Copyright (C) 2020 Truong Bui.
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen
 * Licensed under the MIT License. See 'LICENSE' file for full license information.
 * 
 **********************************************************************************/

#pragma once

#include "Modules/ModuleManager.h"

/**
 * Listener interface for async loading screen module
 */
class ILoadingScreenListenerInterface
{
public:	
	/*
	 * Called in StatupModule to register new modules
	 */
	virtual void OnStartupModule() {};
	/*
	 * Called in ShutdownModule to unregister the modules
	 */
	virtual void OnShutdownModule() {};
};

/**
 * The public base interface for async loading screen module
 */
class ILoadingScreenInterface : public IModuleInterface
{
public:
	/**
	 * Called right after the module DLL has been loaded and the module object has been created
	 */
	virtual void StartupModule() override
	{
		if (!IsRunningCommandlet())
		{
			AddModuleListeners();
			for (int32 i = 0; i < ModuleListeners.Num(); i++)
			{
				ModuleListeners[i]->OnStartupModule();
			}
		}
	}

	/**
	 * Called before the module is unloaded, right before the module object is destroyed.
	 */
	virtual void ShutdownModule() override
	{
		for (int32 i = 0; i < ModuleListeners.Num(); i++)
		{
			ModuleListeners[i]->OnShutdownModule();
		}
	}


	/*
	 * Add new module to ModuleListeners array
	 */
	virtual void AddModuleListeners() {};
protected:
	// List of module listeners
	TArray<TSharedRef<ILoadingScreenListenerInterface>> ModuleListeners;
};