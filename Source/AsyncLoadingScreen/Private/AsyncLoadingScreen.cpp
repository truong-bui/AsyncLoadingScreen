#include "AsyncLoadingScreen.h"

#define LOCTEXT_NAMESPACE "FAsyncLoadingScreenModule"

void FAsyncLoadingScreenModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ILoadingScreenInterface::StartupModule();
}

void FAsyncLoadingScreenModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	ILoadingScreenInterface::ShutdownModule();
}

void FAsyncLoadingScreenModule::AddModuleListeners()
{
	// Add new modules here
}

bool FAsyncLoadingScreenModule::IsGameModule() const
{
	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncLoadingScreenModule, AsyncLoadingScreen)