## Building
Ensure that `cmake` is installed.

```bash
mkdir build &&
cd build &&
cmake .. &&
cmake --build . &&
cmake --install .
```

In the `dist` directory, you will now have binaries called `Warzone_PlayerStrategies` and `Warzone_Tournament`. The `Warzone_PlayerStrategies` binary puts the user in a pre-configured game. Whereas the `Warzone_Tournament` binary allows for entirely custom games and tournaments to be played.

## Running `Warzone_Tournament`
1. From within `dist`, run `./Warzone_Tournament`.
2. Run something along the lines of `tournament -M ../maps/brasil.map ../maps/canada.map -P Aggressive Cheater -G 2 -D 10`

The available strategies are:
```
aggressive
benevolent
neutral
cheater
```

## Running `Warzone_PlayerStrategies`
1. From within `dist`, run `./Warzone_PlayerStrategies`.
2. Follow the instructions in the terminal.