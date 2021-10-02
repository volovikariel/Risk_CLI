# Warzone_Map
## Build
CMake target `Warzone_Map` or files:
* `Map.h`
* `Map.cpp`
* `MapDriver.cpp`

## Usage
```
Usage: provide one or more filepaths as arguments.
If only one filepath is provided:
    The file will be loaded and its load status, continent, and territory details will be displayed.

If multiple filepaths are provided:
    Each file will be loaded and only its load status will be displayed.
```

Ex: `Warzone_Map artic.map` or `Warzone_Map artic.map middleearth.map`

## Testing all the maps
* In the `/maps` directory, run `ls | xargs` to obtain `artic.map brasil.map canada.map discworld.map falklandislands.map hawaii.map ...`
* Run `Warzone_Map` using that long line of filepath arguments
