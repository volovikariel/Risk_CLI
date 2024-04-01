## Demos
### Tournament
https://github.com/volovikariel/Risk_CLI/assets/16262535/bea742a6-136b-4e29-a23a-eec5f8591e10


### PlayerStrategies
https://github.com/volovikariel/Risk_CLI/assets/16262535/4656d308-46fe-4a32-b444-2494942031c1

## Building
Ensure that `cmake` is installed.

```bash
mkdir build &&
cd build &&
cmake .. &&
cmake --build . --target install
```

In the `dist` directory, you will now have binaries called `Warzone_PlayerStrategies` and `Warzone_Tournament`. The `Warzone_PlayerStrategies` binary puts the user in a pre-configured game. Whereas the `Warzone_Tournament` binary allows for entirely custom games and tournaments to be played.

## Running `Warzone_Tournament`
### Running a tournament
1. From within `dist`, run `./Warzone_Tournament`.
2. Run something along the lines of `tournament -M ../maps/brasil.map ../maps/canada.map -P Aggressive Cheater -G 2 -D 10`

The available strategies are:
```
aggressive
benevolent
neutral
cheater
```

### Running a custom game
1. From within `dist`, run `./Warzone_Tournament`.
2. Load a map with `loadmap ../maps/canada.map`
3. Validate the map with `validatemap`
4. Add players with `addplayer testplayer1`, `addplayer testplayer2`
5. Start the game with `gamestart`

## Running `Warzone_PlayerStrategies`
1. From within `dist`, run `./Warzone_PlayerStrategies`.
2. Follow the instructions in the terminal.
