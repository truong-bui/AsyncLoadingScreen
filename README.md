<p align="center">
    <a href="#">
        <img src="blob/master/Resources/Icon256.png">
    </a>
</p>

## Contents
- [About](#about)
- [How it works](#how-it-works)
- [Features](#features)
- [Plugin Architecture](#plugin-architecture)
- [Showcases](#showcases)
- [Requirements](#requirements)
- [Installation](#installation)
- [How to Use](#how-to-use)
- [Wiki](#wiki)
- [License](#license)
- [Contributing](#contributing)
- [Acknowledgment](#acknowledgment)


## About

Loading screens aren't trivial in general, especially in Unreal Engine. And this makes a lot of UE newcomers are confused at first. Because User Widget is destroyed at level transition, and level loading runs on the main thread, it blocks any other game activities until it's completed. That's why you need to use Level Streaming for loading screens otherwise your widget blueprint will not work. You have to manually control which object is loaded/unloaded, you can't use different Game Mode, Player Controller for each level, and sometimes there's still occasional freezing.

That said, you may need to change your game logic to fit with Level Streaming mechanic and it requires a lot of works to achieve a simple thing: Add a Loading Screen. To create a custom loading screen without Level Streaming, you have to do it in Unreal C++. However, this is a hard task for artists and designers, even for developers who are not familiar with the Slate framework and engine module code. Async Loading Screen plug-in comes to a savior.

Async Loading Screen allows you to easily configure a Loading Screen System in the project settings, and automatically add a Loading Screen whenever you open a new level. Async Loading Screen also comes with pre-design UI layouts and default icons that make it easy to custom your loading screen in a few minutes (less than 5 minutes I swear).

## How it works

Async Loading Screen uses MoviePlayer to play a movie at level transition. MoviePlayer is not only playing movie files but also can display a Slate widget on top of the movie. I use this widget for the loading screen.

MoviePlayer is registered to PreLoadMap and PostLoadMapWithWorld delegates so it will be called and shut down automatically by the engine whenever you open a new level.

## Features

- Easy to set up, easy to use.
- Customizable pre-make layouts.
- Customizable Loading icons.
- Lot of default loading icons.
- Supported custom animated images.
- Supported on all major platforms.
- Easily play movies, audio files.
- No need for coding.
- No need for temporary maps or level streaming.
- Automatically handles all level transitions.
- Integrates seamlessly with an existing project.

## Plugin Architecture

--- Comming soon ---

## Showcases

--- Comming soon ---

## Requirements

Async Loading Screen plug-in requires C++ Visual Studio and a C++ code project. If your project is blueprint based then you will need to convert it to C++ project otherwise you cannot packaging your game with code plug-in.

## Installation

--- Comming soon ---

## How to Use

--- Comming soon ---

## Wiki

--- Comming soon ---

## License
Async Loading Screen plug-in is licensed under the [MIT](LICENSE) License.

## Contributing

--- Comming soon ---

## Acknowledgment

Async Loading Screen plug-in is inspired from [Loading Screen](#https://github.com/ue4plugins/LoadingScreen) plugin by Nick Darnell.






