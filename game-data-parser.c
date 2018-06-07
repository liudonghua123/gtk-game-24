#include "game-data-parser.h"

const char *token_type_str[] = {
    "YAML_NO_TOKEN",

    /** A STREAM-START token. */
    "YAML_STREAM_START_TOKEN",
    /** A STREAM-END token. */
    "YAML_STREAM_END_TOKEN",

    /** A VERSION-DIRECTIVE token. */
    "YAML_VERSION_DIRECTIVE_TOKEN",
    /** A TAG-DIRECTIVE token. */
    "YAML_TAG_DIRECTIVE_TOKEN",
    /** A DOCUMENT-START token. */
    "YAML_DOCUMENT_START_TOKEN",
    /** A DOCUMENT-END token. */
    "YAML_DOCUMENT_END_TOKEN",

    /** A BLOCK-SEQUENCE-START token. */
    "YAML_BLOCK_SEQUENCE_START_TOKEN",
    /** A BLOCK-SEQUENCE-END token. */
    "YAML_BLOCK_MAPPING_START_TOKEN",
    /** A BLOCK-END token. */
    "YAML_BLOCK_END_TOKEN",

    /** A FLOW-SEQUENCE-START token. */
    "YAML_FLOW_SEQUENCE_START_TOKEN",
    /** A FLOW-SEQUENCE-END token. */
    "YAML_FLOW_SEQUENCE_END_TOKEN",
    /** A FLOW-MAPPING-START token. */
    "YAML_FLOW_MAPPING_START_TOKEN",
    /** A FLOW-MAPPING-END token. */
    "YAML_FLOW_MAPPING_END_TOKEN",

    /** A BLOCK-ENTRY token. */
    "YAML_BLOCK_ENTRY_TOKEN",
    /** A FLOW-ENTRY token. */
    "YAML_FLOW_ENTRY_TOKEN",
    /** A KEY token. */
    "YAML_KEY_TOKEN",
    /** A VALUE token. */
    "YAML_VALUE_TOKEN",

    /** An ALIAS token. */
    "YAML_ALIAS_TOKEN",
    /** An ANCHOR token. */
    "YAML_ANCHOR_TOKEN",
    /** A TAG token. */
    "YAML_TAG_TOKEN",
    /** A SCALAR token. */
    "YAML_SCALAR_TOKEN"
};

void printParserScanEvent(char *filename) {
    FILE *fh = fopen(filename, "r");
    yaml_parser_t parser;
    yaml_token_t token;

    if (!yaml_parser_initialize(&parser))
        fputs("Failed to initialize parser!\n", stderr);
    if (fh == NULL)
        fputs("Failed to open file!\n", stderr);
    yaml_parser_set_input_file(&parser, fh);

      /* Read tokens */
    int  space = 0;
    while(1) {
        if (!yaml_parser_scan(&parser, &token)) {
        fputs("ERROR: Parsing failed.\n", stderr);
        return;
        }

        if (token.type == YAML_STREAM_END_TOKEN ||
            token.type == YAML_DOCUMENT_END_TOKEN ||
            token.type == YAML_BLOCK_END_TOKEN ||
            token.type == YAML_FLOW_SEQUENCE_END_TOKEN ||
            token.type == YAML_FLOW_MAPPING_END_TOKEN) space >= 4 ? space -= 4 : 0;

        printf("%*s%s(%d)\n", space, "", token_type_str[token.type], token.type);

        if (token.type == YAML_STREAM_START_TOKEN ||
            token.type == YAML_DOCUMENT_START_TOKEN ||
            token.type == YAML_BLOCK_SEQUENCE_START_TOKEN ||
            token.type == YAML_BLOCK_MAPPING_START_TOKEN ||
            token.type == YAML_FLOW_SEQUENCE_START_TOKEN ||
            token.type == YAML_FLOW_MAPPING_START_TOKEN) space += 4;

        /* scan finished */
        if(token.type == YAML_NO_TOKEN ||
        token.type == YAML_STREAM_END_TOKEN
        ) break;
        yaml_token_delete(&token);
    }

    yaml_token_delete(&token);
    yaml_parser_delete(&parser);
    fclose(fh);
}

GameData * parserGameData(char *filename) {
    FILE *fh = fopen(filename, "r");
    yaml_parser_t parser;
    yaml_token_t token;
    GameData *gameData = malloc(sizeof(GameData));
    gameData->count = 0;
    gameData->records = malloc(sizeof(GameDataRecord) * 100);
    GameDataRecord *record;
    int *levelData, *levelDataLocator;
    int blockLevel = 0;
    int startLevelDataParse = 0;
    int state = 0;

    if (!yaml_parser_initialize(&parser))
        fputs("Failed to initialize parser!\n", stderr);
    if (fh == NULL)
        fputs("Failed to open file!\n", stderr);
    yaml_parser_set_input_file(&parser, fh);

    do {
        char **datap;
        char *tk;

        yaml_parser_scan(&parser, &token);
        switch (token.type) {
        case YAML_KEY_TOKEN:
            state = 0;
            break;
        case YAML_VALUE_TOKEN:
            state = 1;
            break;
        case YAML_BLOCK_SEQUENCE_START_TOKEN:
            blockLevel ++;
            break;
        case YAML_BLOCK_MAPPING_START_TOKEN:
            if(blockLevel == 1) {
                record = malloc(sizeof(GameDataRecord));
                record->name = malloc(sizeof(char) * 100);
                record->data = malloc(sizeof(int **));
            }
            blockLevel ++;
            break;
        case YAML_BLOCK_ENTRY_TOKEN:
            startLevelDataParse = (blockLevel == 2) ? 1 : 0;
            break;
        case YAML_BLOCK_END_TOKEN:
            if(blockLevel == 2) {
                record->data = levelData;
                gameData->records[gameData->count] = *record;
                gameData->count ++;
            }
            blockLevel --;
            break;
        case YAML_SCALAR_TOKEN:
            tk = token.data.scalar.value;
            if (startLevelDataParse == 0 && state == 0) {
                /* It's safe to not use strncmp as one string is a literal */
                if (!strcmp(tk, "name")) {
                    datap = &(record->name);
                } else if (!strcmp(tk, "data")) {
                    levelData = malloc(sizeof(int) * 4);
                    levelDataLocator = levelData;
                } else {
                    printf("Unrecognised key: %s\n", tk);
                }
            } else if(startLevelDataParse == 1 && blockLevel == 2) {
                *levelDataLocator = *tk - '0';
                levelDataLocator ++;
            } else if(state == 1) {
                *datap = strdup(tk);
            }
            break;
        default:
            break;
        }
        if (token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);
    } while (token.type != YAML_STREAM_END_TOKEN);

    yaml_token_delete(&token);
    yaml_parser_delete(&parser);
    fclose(fh);
    return gameData;
}