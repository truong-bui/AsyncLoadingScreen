<p align="center">
    <a href="#">
        <img src="/Resources/Icon256.png">
    </a>
</p>

# Contents
- [About](#about)
- [How it works](#how-it-works)
- [Features](#features)
- [Architecture](#architecture)
- [Showcases](#showcases)
- [Requirements](#requirements)
- [Installation](#installation)
    - [Clone from Github](#clone-from-github)
    - [Download from Marketplace](#download-from-marketplace)
- [How to Use](#how-to-use)
- [Wiki](#wiki)
- [License](#license)
- [Acknowledgment](#acknowledgment)


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

# Architecture
Async Loading Screen has four main components: Background, Loading Widget, Tip Widget, and Layout.

## Background
The background is a static image, you can add many images as you want, it will be displayed randomly. Leave it empty if you just want to show a movie instead of a static image.

- Background Settings:

![background-settings](https://user-images.githubusercontent.com/1840964/92411337-a0d2f600-f171-11ea-8960-9c15f54bd62a.png)

## Tip Widget
Just some advice texts that will be shown randomly for players to read while waiting for a new level loaded.

- Tip Widget Settings:

![tip-widget-settings](https://user-images.githubusercontent.com/1840964/92411408-f6a79e00-f171-11ea-8de7-14744375e67a.png)

## Loading Widget
This is the most important component of the plug-in, Loading Widget lets players know that the game is still running and not freezing. 

Loading Widget has two parts: Loading Icon and Loading Text. The Loading Text is just a text shown beside the Loading Icon, we mostly care about the Loading Icon. Async Loading Screen supports 3 types of Loading Icon: Throbber, Circular Throbber, and Image Sequence.

- Loading Widget Settings:

![loading-widget-settings](https://user-images.githubusercontent.com/1840964/92412275-7edb7280-f175-11ea-9ef7-c02f183f5c88.png)

### Loading Icon

- Throbber Settings:

![throbber-settings](https://user-images.githubusercontent.com/1840964/92412332-b9dda600-f175-11ea-986c-852cc93cf61c.png)

- Throbber loading icon example:

![throbber-loading-01](https://user-images.githubusercontent.com/1840964/92412429-10e37b00-f176-11ea-999b-bdeab20cf922.gif) ![throbber-loading-02](https://user-images.githubusercontent.com/1840964/92412452-1fca2d80-f176-11ea-838e-a5ccb55ea72d.gif)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

- Circular Throbber Settings:

![circular-throbber-settings](https://user-images.githubusercontent.com/1840964/92413277-8735ac80-f179-11ea-8e16-ab5e08be1595.png)

- Circular Throbber loading icon example:

![circular-throbber-loading-01](https://user-images.githubusercontent.com/1840964/92412600-a2eb8380-f176-11ea-9a6f-ab162490a9e3.gif) ![circular-throbber-loading-02](https://user-images.githubusercontent.com/1840964/92412750-450b6b80-f177-11ea-8af8-9eb68cea0155.gif)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------
- Image Sequence Settings:

![image-sequence-settings](https://user-images.githubusercontent.com/1840964/92413824-0e841f80-f17c-11ea-896a-3e16e8291967.png)

- Image Sequence loading icon examples:

![image-sequence-loading-03](https://user-images.githubusercontent.com/1840964/92413857-2cea1b00-f17c-11ea-9d59-9cd03f1ed069.gif)
![image-sequence-loading-01](https://user-images.githubusercontent.com/1840964/92413859-2fe50b80-f17c-11ea-9254-74ebd073a0eb.gif)
![image-sequence-loading-02](https://user-images.githubusercontent.com/1840964/92413848-26f43a00-f17c-11ea-8f72-c658e3f904a0.gif)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Layout
This is the way Async Loading Screen widgets are arranged on the screen. In version 1.0 Async Loading Screen supports 5 different Layouts: Classic, Center, Letterbox, Sidebar, and Dual Sidebar.

- Layout Settings:

![layout-settings](https://user-images.githubusercontent.com/1840964/92414921-af74d980-f180-11ea-9957-7032f171e843.png)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

### Classic Layout
- Classic layout examples:

![classic-layout-01](https://user-images.githubusercontent.com/1840964/92415877-14cac980-f185-11ea-870a-dc8a7e7d68ea.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

### Center Layout 
- Center layout examples:

![center-layout-01](https://user-images.githubusercontent.com/1840964/92416035-c833be00-f185-11ea-8d7d-625b4166ca26.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

### Letterbox Layout
- Letterbox layout examples:

![letterbox-layout-01](https://user-images.githubusercontent.com/1840964/92416029-c2d67380-f185-11ea-92f9-f7f9021b825f.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

### Sidebar Layout
- Sidebar layout examples:

![sidebar-layout-01](https://user-images.githubusercontent.com/1840964/92416031-c4a03700-f185-11ea-9938-b63f1a2971fa.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

### Dual Sidebar Layout
- Dual Sidebar layout examples:

![dual-sidebar-layout-01](https://user-images.githubusercontent.com/1840964/92416037-c964eb00-f185-11ea-8ed4-086bb49fc52a.jpg)

---------------------------------------------------------------------------------------------------------------------------------------------------------------------

# Showcases
If you're using Async Loading Screen plugin in your game and want to showcase here, shoot me an email about your game's info, including screenshots(gif/png/jpeg) of your in-game loading screen.

# Requirements

Async Loading Screen plug-in requires Visual Studio and a C++ code project. If your project is blueprint based then you will need to convert it to C++ project otherwise you cannot package your game with code plug-in.

# Installation

> :warning: **After installed Async Loading Screen plugin, the "Project/Movies" setting won't work anymore. You need to set up your startup movies in the 'Startup Loading Screen' section of the plugin's setting.**

- ## Clone from Github
Clone this repository into your project's /Plugins folder, make one if it doesn’t exist, then compile your game in Visual Studio. If your project is blueprint based you’ll need to convert it to a c++ project by adding any c++ class.

The plug-in will be enabled by default. You can disable it in “Edit→Plugins→Project/Loading Screen”. You configure the plug-in in project settings "Game/Async Loading Screen".

![async-loading-screen-screenshot-01](https://user-images.githubusercontent.com/1840964/91760594-d02bb500-ebfd-11ea-85fc-08c5a556a903.png)

- ## Download from Marketplace
--- Coming soon ---
# How to Use

--- Coming soon ---

# Wiki

--- Coming soon ---

# License
Async Loading Screen plug-in is licensed under the [MIT](LICENSE) License.

# Acknowledgment

Async Loading Screen plug-in is inspired from [Loading Screen](https://github.com/ue4plugins/LoadingScreen) plug-in by Nick Darnell.






