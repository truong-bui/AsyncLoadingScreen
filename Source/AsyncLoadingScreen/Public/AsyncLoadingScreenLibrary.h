/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AsyncLoadingScreenLibrary.generated.h"

/**
 * Async Loading Screen Function Library
 */
UCLASS()
class ASYNCLOADINGSCREEN_API UAsyncLoadingScreenLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	static int32 DisplayBackgroundIndex;
	static int32 DisplayTipTextIndex;
	static int32 DisplayMovieIndex;
	static bool  bShowLoadingScreen;
	static bool  bHoldLoadingScreen;
public:
	
	/**
	 * Set which background will be displayed on the loading screen by index. The "SetDisplayBackgroundManually" option in Background setting needs to be "true" to use this function.
	 * 
	 * @param BackgroundIndex Valid index of the Background in "Images" array in Background setting. If the index is not valid, then it will display random background instead.
	 **/
	UFUNCTION(BlueprintCallable, Category = "Async Loading Screen")
	static void SetDisplayBackgroundIndex(int32 BackgroundIndex);

	/**
	 * Set which text will be displayed on the loading screen by index. The "SetDisplayTipTextManually" option in Tip Widget setting needs to be "true" to use this function.
	 *
	 * @param TipTextIndex Valid index of the text in "TipText" array in Tip Widget setting. If the index is not valid, then it will display random text instead.
	 **/
	UFUNCTION(BlueprintCallable, Category = "Async Loading Screen")
	static void SetDisplayTipTextIndex(int32 TipTextIndex);

	/**
	 * Set which movie will be displayed on the loading screen by index. The "SetDisplayMovieIndexManually" option needs to be "true" to use this function.
	 *
	 * @param MovieIndex Valid index of the movie in "MoviePaths" array.
	 **/
	UFUNCTION(BlueprintCallable, Category = "Async Loading Screen")
	static void SetDisplayMovieIndex(int32 MovieIndex);


	/**
	 * Set enable/disable the loading screen for next levels
	 *
	 * @param bIsEnableLoadingScreen Should we enable the loading screen for next level?
	 **/
	UFUNCTION(BlueprintCallable, Category = "Async Loading Screen")
	static void SetEnableLoadingScreen(bool bIsEnableLoadingScreen);


	/**
	 * Get enable/disable the loading screen for next levels
	 *
	 **/
	UFUNCTION(BlueprintPure, Category = "Async Loading Screen")
	static inline bool GetIsEnableLoadingScreen() { return bShowLoadingScreen; }

	/**
	 * Stop the loading screen. To use this function, you must enable the "bAllowEngineTick" option.
	 * Call this function in BeginPlay event to stop the Loading Screen (works with Delay node).
	 *
	 **/
	UFUNCTION(BlueprintCallable, Category = "Async Loading Screen")
	static void StopLoadingScreen();

	/**
	 * Keep the loading screen up for as long as you need, on top of the plugin's own conditions. Use this when your game
	 * knows better than the plugin when it is ready, for example when it still has to download or stream in content after
	 * joining a server. To use this function, you must enable the "Hold Until Network Ready" option in the Multiplayer settings.
	 *
	 * Call it with "true" before opening the level, then with "false" (or call "StopLoadingScreen") once your game is ready.
	 * The "Network Max Wait Time" safety timeout still applies, and the hold is automatically cleared when the loading
	 * screen closes, so it never leaks into the next level.
	 *
	 * @param bHold Should the loading screen be held?
	 **/
	UFUNCTION(BlueprintCallable, Category = "Async Loading Screen")
	static void SetLoadingScreenHold(bool bHold);

	/**
	 * Is the loading screen currently being held by "SetLoadingScreenHold"?
	 **/
	UFUNCTION(BlueprintPure, Category = "Async Loading Screen")
	static inline bool GetIsLoadingScreenHold() { return bHoldLoadingScreen; }

	/**
	 * Display the Default Loading Screen right now, outside of a level transition, for example while your game loads assets
	 * or a save game. The screen stays up until you call "StopLoadingScreen", and the game keeps running underneath it.
	 *
	 * NOTE: Do not call this right before "Open Level" or "Client Travel". The engine starts its own loading screen when the
	 * level load begins, which would run a second loading screen on top of this one. Use the Multiplayer settings to cover
	 * level transitions instead.
	 **/
	UFUNCTION(BlueprintCallable, Category = "Async Loading Screen")
	static void ShowLoadingScreen();

	static inline int32 GetDisplayBackgroundIndex() { return DisplayBackgroundIndex; }
	static inline int32 GetDisplayTipTextIndex() { return DisplayTipTextIndex; }
	static inline int32 GetDisplayMovieIndex() { return DisplayMovieIndex; }

};
