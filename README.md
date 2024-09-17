<p align="center">
    <a href="#">
        <img src="/Resources/Icon256.png">
    </a>
</p>

# Contents
- [Trailer](#trailer)
- [About](#about)
- [How it works](#how-it-works)
- [Features](#features)
- [Changelog](#changelog)
- [Requirements](#requirements)
- [Installation](#installation)
    - [Clone from Github](#clone-from-github)
    - [Download from Marketplace](#download-from-marketplace)
    - [Setup](#setup)
- [Architecture](#architecture)
- [Tutorial](#tutorial)
    - [1) Create Testing Levels and Blueprints](#1-create-testing-levels-and-blueprints)
    - [2) Gathering images and movies](#2-gathering-images-and-movies)
    - [3) Setup Startup Movies](#3-setup-startup-movies)
    - [4) Configure Default Loading Screen setting](#4-configure-default-loading-screen-setting)        
        - [4.1) Default Loading Screen - Background setting](#41-default-loading-screen---background-setting)
        - [4.2) Default Loading Screen - Tip Widget setting](#42-default-loading-screen---tip-widget-setting)
        - [4.3) Default Loading Screen - Loading Widget setting](#43-default-loading-screen---loading-widget-setting)
    - [5) Configure Classic Layout setting](#5-configure-classic-layout-setting)
    - [6) Image Sequence Settings](#6-image-sequence-settings)
    - [7) Set proper background and tip for each level](#7-set-proper-background-and-tip-for-each-level)
- [Showcases](#showcases)
- [Donate](#donate)
- [License](#license)
- [Acknowledgment](#acknowledgment)


# Trailer

[![Unreal Engine Async Loading Screen Plugin](http://i3.ytimg.com/vi/3rwsNYPUr0I/hqdefault.jpg)](https://youtu.be/3rwsNYPUr0I "Unreal Engine Async Loading Screen Plugin")

# About

Loading screens aren't trivial in general, especially in Unreal Engine. And this makes a lot of UE newcomers are confused at first. Due to User Widget is destroyed at level transition, and level loading runs on the main thread, it blocks any other game activities until it's completed. That's why you need to use Level Streaming for loading screens otherwise your widget blueprint will not work. You have to manually control which object is loaded/unloaded, you can't use different Game Mode, Player Controller for each level, and sometimes there's still occasional freezing.

That said, you may need to change your game logic to fit with Level Streaming mechanic and it requires a lot of works to achieve a simple thing: Add a Loading Screen. To create a custom loading screen without Level Streaming, you have to do it in Unreal C++. However, this is a hard task for artists and designers, even for developers who are not familiar with the Slate framework and engine module code. Async Loading Screen plug-in comes to a savior.

Async Loading Screen allows you to easily configure a Loading Screen System in the project settings, and automatically add a Loading Screen whenever you open a new level. Async Loading Screen also comes with pre-design UI layouts and default icons that make it easy to custom your loading screen in a few minutes (less than 5 minutes I swear).

# How it works

Async Loading Screen uses MoviePlayer to play a movie at level transition, on a different thread than game thread. MoviePlayer is not only playing movie files but also can display a Slate widget on top of the movie. We use this widget for the loading screen.

MoviePlayer is registered to PreLoadMap and PostLoadMapWithWorld delegates so it will be called and shut down automatically by the engine whenever you open a new level.

# Features

- Easy to set up, easy to use.
- Customizable pre-make responsive layouts.
- Customizable loading icons.
- Pre-make default loading icons.
- Supported animating images sequence.
- Supported on all major platforms.
- Easily play movies, audio files.
- No need for coding.
- No temporary maps, no level streaming.
- Automatically handles all level transitions.
- Integrates seamlessly with an existing project.

# Changelog

### Version 1.5.0 (6/4/2023)
- Add "PreloadBackgroundImages" option
- Add Blueprint functions "PreloadBackgroundImages" and "RemovePreloadedBackgroundImages" to load and remove all the background images.

![image](https://user-images.githubusercontent.com/1840964/230429985-6d353486-0389-448c-8e37-d4f9dd3505ae.png)
![image](https://user-images.githubusercontent.com/1840964/230430019-fb5ab524-7f59-4201-b263-00cbc17672f0.png)

### Version 1.4.0 (25/1/2023)
- Fixed unintialized errors
- Add Blueprint function "SetEnableLoadingScreen" to show/hide Loading Screen. Call this function before call the "OpenLevel" node

![ALS-ShowHideLoadingScreen](https://user-images.githubusercontent.com/1840964/214602739-ebcdd994-08a4-4837-96b5-c5596efbdcb1.png)

### Version 1.3.0 (29/08.2021)
- Supports Unreal Engine 4.27 (Select other branches for other UE versions)
- Added option for hiding Loading Widget when level loading completes
![image](https://user-images.githubusercontent.com/1840964/131251300-dbf13ed7-26ae-4965-a119-7b51b29dfe52.png)
- Moved animating Image Sequence into OnPaint function
---
### Version 1.2 (11/01.2021)
- Added new blueprint library function **Stop Loading Screen**. To use this function, you must enable the **Allow Engine Tick** option. Call this function in **BeginPlay** event to stop the Loading Screen (works with Delay node):

![async-loading-screen-01](https://user-images.githubusercontent.com/1840964/104131830-a3f22300-53ab-11eb-8543-3e8ba6ef2609.png)

- Added **Show Loading Complete Text** option and **Loading Complete Text Settings** section, which show a text when level loading is complete. Note that you need to set **Wait For Manual Stop** = true, and **Minimum Loading Screen Display Time** = -1, this also allows players press any key to stop the Loading Screen by themself.

Show Loading Complete Text Settings:
![async-loading-screen-02](https://user-images.githubusercontent.com/1840964/104131945-896c7980-53ac-11eb-870a-b7987364f00c.png)

Example:
![async-loading-screen-03](https://user-images.githubusercontent.com/1840964/104131958-ad2fbf80-53ac-11eb-92a7-8951cbe2061e.png)

---

### Version 1.1 (14/09/2020):
- Added Shuffle option in the movie setting.
- Implemented set specific background/tip/movie for each level.
- Added SetDisplayBackgroundManually, SetDisplayTipTextManually, SetDisplayMovieManually options.
- Changed TipWidgetVerticalPadding to BorderVerticalOffset in Center Layout settings.
- Changed WidgetHorizontalPadding to BorderHorizontalOffset in Sidebar Layout settings.
- Add Async Loading Screen Function Library with 3 Blueprint functions:
![function-library-01](https://user-images.githubusercontent.com/1840964/93088595-577c2c80-f6c4-11ea-8f23-ec9d065277dc.png)

# Requirements

~~Async Loading Screen plug-in requires Visual Studio and a C++ code project. If your project is blueprint based then you will need to convert it to C++ project otherwise you cannot package your game with code plug-in.~~ It doesn't require c++ project anymore.

# Installation

> :warning: **After installed Async Loading Screen plugin, the "Project/Movies" setting won't work anymore. You need to set up your startup movies in the 'Startup Loading Screen' section of the plugin's setting.**

- ## Clone from Github
Clone this repository into your project's /Plugins folder, make one if it doesn’t exist, then compile your game in Visual Studio. If your project is blueprint based you’ll need to convert it to a c++ project by adding any c++ class. Supports Engine version from 4.23 and above.

- ## Download from Marketplace
Download Async Loading Screen from [Marketplace](https://www.unrealengine.com/marketplace/en-US/product/async-loading-screen) and enable the plugin in Plugins.

- ## Setup
You can disable/enable the plugin in **Edit→Plugins→Project/Loading Screen**. You configure the plug-in settings in project settings **Project Settings/Game/Async Loading Screen**.

![async-loading-screen-screenshot-01](https://user-images.githubusercontent.com/1840964/91760594-d02bb500-ebfd-11ea-85fc-08c5a556a903.png)

# Architecture
Async Loading Screen has four main components: Background, Loading Widget, Tip Widget, and Layout.

## Background
The background is a static image, you can add many images as you want, it will be displayed randomly (unless **Set Display Background Manually** option is enabled). Leave it empty if you just want to show a movie instead of a static image.

- Background Setting:

![background-settings](https://user-images.githubusercontent.com/1840964/93088749-8397ad80-f6c4-11ea-9a85-5399efea3ddd.png)

## Tip Widget
Just some useful texts that will be shown randomly (unless **Set Display Tip Text Manually** option is enabled) for players to have something to look at while waiting for a new level loaded.

- Tip Widget Setting:

![tip-widget-settings](https://user-images.githubusercontent.com/1840964/93089000-df623680-f6c4-11ea-82fb-e2a44f9a524f.png)

## Loading Widget
This is the most important component of the plug-in, Loading Widget lets players know that the game is still running and not freezing. 

Loading Widget has two parts: Loading Icon and Loading Text. The Loading Text is just a text shown beside the Loading Icon, we mostly care about the Loading Icon. Async Loading Screen supports 3 types of Loading Icon: Throbber, Circular Throbber, and Image Sequence.

- Loading Widget Setting:

![loading-widget-settings](https://user-images.githubusercontent.com/1840964/92412275-7edb7280-f175-11ea-9ef7-c02f183f5c88.png)

### Loading Icon

- Throbber Setting:

![throbber-settings](https://user-images.githubusercontent.com/1840964/92412332-b9dda600-f175-11ea-986c-852cc93cf61c.png)

- Throbber loading icon example:

![throbber-loading-01](https://user-images.githubusercontent.com/1840964/92412429-10e37b00-f176-11ea-999b-bdeab20cf922.gif) ![throbber-loading-02](https://user-images.githubusercontent.com/1840964/92412452-1fca2d80-f176-11ea-838e-a5ccb55ea72d.gif)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

- Circular Throbber Setting:

![circular-throbber-settings](https://user-images.githubusercontent.com/1840964/92413277-8735ac80-f179-11ea-8e16-ab5e08be1595.png)

- Circular Throbber loading icon example:

![circular-throbber-loading-01](https://user-images.githubusercontent.com/1840964/92412600-a2eb8380-f176-11ea-9a6f-ab162490a9e3.gif) ![circular-throbber-loading-02](https://user-images.githubusercontent.com/1840964/92412750-450b6b80-f177-11ea-8af8-9eb68cea0155.gif)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------
- Image Sequence Setting:

![image-sequence-settings](https://user-images.githubusercontent.com/1840964/92413824-0e841f80-f17c-11ea-896a-3e16e8291967.png)

- Image Sequence loading icon examples:

![image-sequence-loading-03](https://user-images.githubusercontent.com/1840964/92539810-c98be600-f26c-11ea-832c-770af9438050.gif)
![image-sequence-loading-01](https://user-images.githubusercontent.com/1840964/92413859-2fe50b80-f17c-11ea-9254-74ebd073a0eb.gif)
![image-sequence-loading-02](https://user-images.githubusercontent.com/1840964/92413848-26f43a00-f17c-11ea-8f72-c658e3f904a0.gif)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Layout
This is the way Async Loading Screen widgets are arranged on the screen. In version 1.0 Async Loading Screen supports 5 different Layouts: Classic, Center, Letterbox, Sidebar, and Dual Sidebar.

- Layout Settings:

![layout-settings](https://user-images.githubusercontent.com/1840964/92414921-af74d980-f180-11ea-9957-7032f171e843.png)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

- Classic layout example:

![classic-layout-01](https://user-images.githubusercontent.com/1840964/92415877-14cac980-f185-11ea-870a-dc8a7e7d68ea.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

- Center layout example:

![center-layout-01](https://user-images.githubusercontent.com/1840964/92416035-c833be00-f185-11ea-8d7d-625b4166ca26.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

- Letterbox layout example:

![letterbox-layout-01](https://user-images.githubusercontent.com/1840964/92416029-c2d67380-f185-11ea-92f9-f7f9021b825f.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

- Sidebar layout example:

![sidebar-layout-01](https://user-images.githubusercontent.com/1840964/92416031-c4a03700-f185-11ea-9938-b63f1a2971fa.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

- Dual Sidebar layout example:

![dual-sidebar-layout-01](https://user-images.githubusercontent.com/1840964/92416037-c964eb00-f185-11ea-8ed4-086bb49fc52a.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

# Tutorial
In this tutorial, I will use a blank C++ project with UE version 4.25. But you can freely integrate Async Loading Screen plugin into your existing projects without problems. You should install the plugin following [Installation](#installation) steps above.

## 1) Create Testing Levels and Blueprints
- First, we create two empty levels named **Level_1** and **Level_2** for testing purposes:

![tutorial-02](https://user-images.githubusercontent.com/1840964/92516241-1c4ea900-f23f-11ea-9e32-76b67f2c96a9.png)

- Create BP_GameMode, BP_PlayerController, and an UMG blueprint WBP_OpenLevelButton:

![tutorial-03](https://user-images.githubusercontent.com/1840964/92516572-95e69700-f23f-11ea-9a81-c9e4fb9fa0e4.png)

- In WBP_OpenLevelButton, create a button at the center of the screen:

![tutorial-04](https://user-images.githubusercontent.com/1840964/92516863-fece0f00-f23f-11ea-81f4-6dabd9c05d66.png)

- In WBP_OpenLevelButton graph, add the following nodes in the button's OnClick event:

![tutorial-05](https://user-images.githubusercontent.com/1840964/92517440-dd215780-f240-11ea-855c-0f41e8f66046.png)

- In BP_PlayerController, create that widget and add to the viewport:

![tutorial-06](https://user-images.githubusercontent.com/1840964/92517611-1eb20280-f241-11ea-9bc1-f84521432e41.png)

- Also enable "Show Mouse Cursor" option in BP_PlayerController: 

![tutorial-07](https://user-images.githubusercontent.com/1840964/92517958-cb8c7f80-f241-11ea-83fc-2a74ecdbc4db.png)

- Change default GameMode to our BP_GameMode and BP_PlayerController in "Project/Maps & Modes" setting:

![tutorial-08](https://user-images.githubusercontent.com/1840964/92518004-dfd07c80-f241-11ea-900d-dd89e4163325.png)

- Add two maps to the included list in "Projects/Packaging" setting so you don't have to see annoying warnings when Launch:

![tutorial-09](https://user-images.githubusercontent.com/1840964/92518328-5cfbf180-f242-11ea-9b8b-e7452dff4454.png)

### **Note that to see the plugin is actually working, you need to play as Standalone Game or Quick Launch. Play in Editor (PIE) will not work due to different level managing of Play in Editor and packaged game:**

![image](https://github.com/truong-bui/AsyncLoadingScreen/assets/1840964/ab1f4cd1-a1f5-4476-8a29-0a30da32943e)

If you play Standalone Game or Launch the game, you will see a black screen with a white panel showing up very quickly. We'll need a little setting up to make the loading screen look better.

---

## 2) Gathering images and movies
You can use your own assets instead but for this tutorial, I'll use Unreal Engine 5 Wallpapers Desktop from this [post](https://forums.unrealengine.com/unreal-engine/announcements-and-releases/1760020-share-your-excitement-for-unreal-engine-5-with-free-wallpapers-gifs-and-more) and UE4 Startup Video Files from this [page](https://www.unrealengine.com/en-US/branding). Here are links download for you:
- [UE5 Wallpapers - Desktop](https://epicgames.ent.box.com/s/zuorr34ojf3010k71tiv2c79s266v12b/folder/112871747507)
- [UE4 Startup Video Files](https://epicgames.ent.box.com/s/t94yqy3lxb3mlytmqf5znrr2pobus3fm)

- Create a "Movies" folder in the "Content" folder and copy your movie files into the "Content/Movies" folder (don't import movies into the project just copy the original movie files):

![tutorial-11](https://user-images.githubusercontent.com/1840964/92527405-24afdf80-f251-11ea-97f0-ebe893e27986.png)

- Import your background images into the project:

![tutorial-12](https://user-images.githubusercontent.com/1840964/92527677-96882900-f251-11ea-8cb3-b99c41925337.png)

---

## 3) Setup Startup Movies

Now go to "Game/Async Loading Screen" setting in the Project Settings and open "Startup Loading Screen" section. This Startup Loading Screen only shows once when the player first opens the game so usually, you don't want to show a loading screen here. Instead, you'll want to show a UE4 logo intro movie, your studio logo movie, your sponsor 1 logo movie, your sponsor 2 logo movie, or some weird shit movies, depending on you.

- Uncheck "Auto Complete when Loading Completes" option so we can see the movies play till end
- Add your movie path without extension in "Movie Paths" array

Here is the Startup Loading Screen setting:

![tutorial-13](https://user-images.githubusercontent.com/1840964/92529902-8eca8380-f255-11ea-92a4-a1c3ed376d62.png)

Click play Standalone Game or Launch button, you will see UE4 logo movie show up here.

---

## 4) Configure Default Loading Screen setting

Open "Default Loading Screen" setting, here we actually set up a loading screen for our game. You can leave most of options default but for testing purpose, we'll change some options.

- Set "Minimum Loading Screen Display Time" value to 10 (I recommend setting it to -1 in packaged games)
- Set "Layout" to Classic for this tutorial.

Here is Default Loading Screen setting:

![tutorial-14](https://user-images.githubusercontent.com/1840964/92531295-33e65b80-f258-11ea-98d7-489521a93a87.png)

### 4.1) Default Loading Screen - Background setting

Open "Background" setting in the Default Loading Screen section. Add your background images to the Images array. You can leave other options default:

![tutorial-15](https://user-images.githubusercontent.com/1840964/92533170-13b89b80-f25c-11ea-9542-3947df9a0079.png)

### 4.2) Default Loading Screen - Tip Widget setting

Open "Tip Widget" setting in the Default Loading Screen. Add some useful texts, you can change text's appearance like color, font, shadow effect here:

![tutorial-16](https://user-images.githubusercontent.com/1840964/92533302-5b3f2780-f25c-11ea-9975-24f0c09c6e03.png)

### 4.3) Default Loading Screen - Loading Widget setting
Open "Loading Widget" setting in the Default Loading Screen, here we will custom our loading icon. If you open AsyncLoadingScreen Content folder, you can see there are a lot of default loading images I had made. We'll use those default images in this tutorial.

![tutorial-17](https://user-images.githubusercontent.com/1840964/92533833-81b19280-f25d-11ea-91b5-239bea77ba1a.png)

- Set "Loading Icon Type" to "Throbber"
- Set "Loading Widget Type" to "Horizontal"
- Open "Throbber Settings" and select icon-cat-print in "Image" option
- Set "Space" value to 20

You can leave other options default.

![tutorial-18](https://user-images.githubusercontent.com/1840964/92534376-ce499d80-f25e-11ea-8a13-98ef026b34bf.png)

---
## 5) Configure Classic Layout setting
Because we already chose the Classic layout in previous step so we will configure the Classic layout and ignore other layouts. Open "Classic" setting in Layout category and change following options:

- Set "Border Padding" to 20
- Open "Border Background" option and set Tint color (R:0.1, G:0.1, B:0.1, A:0.8)

You can leave other options default.

![tutorial-19](https://user-images.githubusercontent.com/1840964/92535368-4749f480-f261-11ea-9fe5-76ff2e5dd215.png)

---

Alright! Now you can click play Standalone Game or Launch button to see your loading screen.

![tutorial-20](https://user-images.githubusercontent.com/1840964/92536073-f6d39680-f262-11ea-8c42-8f540aea6217.jpg)


## 6) Image Sequence Settings
Async Loading Screen also supports custom loading icon using Image Sequence. Open "Loading Widget" and change following options:

- Set "Loading Icon Type" to "Image Sequence"
- Open "Image Sequence Settings" and add all images in folder "Default/Mannequin/RunImages" to Images array

You can leave other options default.

![tutorial-21](https://user-images.githubusercontent.com/1840964/92536598-48c8ec00-f264-11ea-8e35-73b14364e3bb.png)

Click play or Launch button and you'll see the Mannequin is running as a loading icon:

![tutorial-22](https://user-images.githubusercontent.com/1840964/92536933-0eac1a00-f265-11ea-8d62-c173e632b55a.jpg)

---
## 7) Set proper background and tip for each level:
In updated version 1.1, Async Loading Screen supports display specific background/tip/movie for each level as you want. In this tutorial we'll show proper background and tip for **Level_1** and **Level_2**:

- Open **Background** setting and enable **Set Display Background Manually**.
![tutorial-23](https://user-images.githubusercontent.com/1840964/93087118-534f0f80-f6c2-11ea-94e3-4d3c0fc27c55.png)

- Open **Tip Widget** setting, enable **Set Display Tip Text Manually** and add 2 more following texts in **TipText** array:
![tutorial-24](https://user-images.githubusercontent.com/1840964/93087270-842f4480-f6c2-11ea-9e8f-d79ff58dd35b.png)

- Open **WBP_OpenLevelButton** blueprint, in On Clicked event, set **Background Index=4, Tip Text Index=2** before **Open Level=Level_2** and **Background Index=2, Tip Text Index=1** before **Open Level=Level_1**:
![tutorial-25](https://user-images.githubusercontent.com/1840964/93087474-ceb0c100-f6c2-11ea-8502-5a5615fd15eb.png)

- Now click play Standalone Game or Launch button, you'll see Async Loading Screen always show Background index 4, TipText index 2 when loading Level_2, and Background index 2, TipText index 1 when loading Level_1.

---

Congratulation! We have finished the tutorial. You should try other layouts and settings to see which is fit for your game.


# Showcases

- [Nova Assault](https://etereagames.com/novaassault/)

![Nova Assault](https://github.com/user-attachments/assets/443bd26c-f015-45b8-8ea1-6bf4f25fdc32)

---

- [Colors:Zero](http://www.colors-zero.com/)

![Colors:Zero](https://user-images.githubusercontent.com/1840964/233838182-2c8cd8fa-dc49-41ed-b536-27a8650fe98a.jpg)

---

- [Elven Warmaiden](https://store.steampowered.com/app/1878000/Elven_Warmaiden/)

![Elven Warmaiden](https://cdn.akamai.steamstatic.com/steam/apps/1878000/header.jpg?t=1644526061)

---
- [Logic - Keypad](https://store.steampowered.com/app/2265160/Logic__Keypad/)

![logic-keypad](https://user-images.githubusercontent.com/1840964/215274878-c6dd3aa1-6b9c-4f4a-af41-de083943c8ef.jpg)

---
- [Witchtastic](https://store.steampowered.com/app/1080010/Witchtastic/)

![witchtastic](https://user-images.githubusercontent.com/1840964/198943299-529b153a-322d-4431-bec9-9046147ced43.jpg)

---
- [Cursed Spire](https://www.youtube.com/watch?v=ZZlxmdRDRMI)

![cursed-spire-async-loading-screen](https://user-images.githubusercontent.com/1840964/93096502-ac24a500-f6ce-11ea-9f6a-613c7a6bb926.gif)

---

- [Pumpkin Jack](https://www.pumpkin-jack.com/)

![pumpkin-jack](https://user-images.githubusercontent.com/1840964/99428459-41475280-2939-11eb-86a5-b27f39b73037.png)

---
- [Cions of Vega](https://tongucbodur.com/vega)

![cions-of-vega](https://user-images.githubusercontent.com/1840964/120698381-2d300080-c4d9-11eb-96bd-9c0fe71f6813.jpg)

---
- [BrightSeeker](https://store.steampowered.com/app/1491950/BrightSeeker/)

![BrightSeeker_Scr_(4)](https://user-images.githubusercontent.com/1840964/133012031-f5e91478-5e32-470d-9c8c-df6f13fcf630.png)

---
If you're using Async Loading Screen plugin in your game and would like to showcase here, shoot me an email about your game's info, including screenshots(gif/png/jpeg/video) of your in-game loading screen.

# Donate
If Async Loading Screen is useful for you or your team, you can [buy me a coffee](https://www.buymeacoffee.com/truongbui) to support this project.

# License
Async Loading Screen plug-in is licensed under the [MIT](LICENSE) License.

# Acknowledgment

Async Loading Screen plug-in is inspired from [Loading Screen](https://github.com/ue4plugins/LoadingScreen) plug-in by Nick Darnell.
