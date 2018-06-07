#ifndef GAME_H
#define GAME_H

#include <gtk/gtk.h>
#include <stdlib.h>

void initializationGame();

void initializationGameLevel(int level);

void initializationGameResetLevel();

void initializationGameNextLevel();

void initializationGamePreviousLevel();

void btnNumCommon(int position);

void btnOperationCommon(int position);

#endif // !GAME_H