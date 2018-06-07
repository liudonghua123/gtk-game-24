#ifndef GAME_DATA_PARSER
#define GAME_DATA_PARSER

#include <stdio.h>
#include <yaml.h>

typedef struct GameDataRecord {
    char *name;
    int *data;
} GameDataRecord;
typedef struct GameData {
    int count;
    GameDataRecord * records;
} GameData;

GameData * parserGameData(char *filename);

#endif // !GAME_DATA_PARSER
