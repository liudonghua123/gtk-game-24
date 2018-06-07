#include "game.h"
#include "game-data-parser.h"

extern GtkWidget *window;
extern GtkWidget *lblLevelName;
extern GtkWidget *btnNum1;
extern GtkWidget *btnNum2;
extern GtkWidget *btnNum3;
extern GtkWidget *btnNum4;
extern GtkWidget *btnPlus;
extern GtkWidget *btnMinus;
extern GtkWidget *btnMultiple;
extern GtkWidget *btnDivide;
extern GtkWidget *btn24;
extern GtkWidget *lblStatus;
extern GtkWidget ** btnNums;
extern GtkWidget ** btnOperations;

#define MAX_LEVEL_NAME_LENGTH 100

int maxGameLevels = 4;
int **allLevelData;
char **allLevelName;
int currentLevel = 0;

int *gameInitialData;
int *gameData;
int opeartionStack[] = {0, 0, 0};
int operandStack[] = {-1, -1};
gboolean gameOver = FALSE;
gboolean startOperation = FALSE;
int currentCalcuationBtnPosition = -1;

char* intToCharString(int num) {
    char* str = malloc(sizeof(char) * 3);
    // sprintf(str, "%d", num);
    // convert 123 to string [buf]
    itoa(num, str, 10);
    return str;
}

int charStringToInt(char* str) {
    return atoi(str);
}


void initOpeartionStack() {
    opeartionStack[0] = 0;
    opeartionStack[1] = 0;
    opeartionStack[2] = 0;
}

void initOperandStack() {
    operandStack[0] = -1;
    operandStack[1] = -1;
}

void initializationGameData(int level) {
    // 初始化 btnNums/btnOperations
    btnNums = malloc(sizeof(GtkWidget *) * 4);
    btnNums[0] = btnNum1;
    btnNums[1] = btnNum2;
    btnNums[2] = btnNum3;
    btnNums[3] = btnNum4;
    btnOperations = malloc(sizeof(GtkWidget *) * 4);
    btnOperations[0] = btnPlus;
    btnOperations[1] = btnMinus;
    btnOperations[2] = btnMultiple;
    btnOperations[3] = btnDivide;
    gameInitialData = allLevelData[level];
    gameData = (int *)malloc(sizeof(int) * 4);
    for(int i = 0; i < 4; i ++) {
        gameData[i] = gameInitialData[i];
        gtk_button_set_label(GTK_BUTTON(btnNums[i]), intToCharString(gameData[i]));
        gtk_widget_show(GTK_WIDGET(btnNums[i]));
    }
    gtk_label_set_label(GTK_LABEL(lblLevelName), allLevelName[level]);
}

void toggleNumButton(gboolean enable) {
    for(int i = 0; i < 4; i ++) {
        gtk_widget_set_sensitive(btnNums[i], enable);
    }
    for(int i = 0; i < 4; i ++) {
        gtk_widget_set_sensitive(btnOperations[i], !enable);
    }
}

void updateUI() {
    for(int i = 0; i < 4; i ++) {
        gtk_button_set_label(GTK_BUTTON(btnNums[i]), intToCharString(gameData[i]));
    }
}

void checkGameOver() {
    int btnNumsVisiable = 0;
    for(int i = 0; i < 4; i ++) {
        if(gtk_widget_get_visible(btnNums[i])) {
            btnNumsVisiable ++;
        }
    }
    if(btnNumsVisiable == 1) {
        if(charStringToInt(gtk_button_get_label(GTK_BUTTON(btnNums[currentCalcuationBtnPosition])))==24) {
            gtk_label_set_label(GTK_LABEL(lblStatus), "game success!");
        }
        else {
            gtk_label_set_label(GTK_LABEL(lblStatus), "game over!");
        }
    }
    else {
        gtk_label_set_label(GTK_LABEL(lblStatus), "game cotinue!");
    }
}

void initializationGame() {
    GameData * gameData = parserGameData("game-data.yaml");
    maxGameLevels = gameData->count;
    allLevelData = malloc(sizeof(int *) * maxGameLevels);
    allLevelName = malloc(sizeof(char) * MAX_LEVEL_NAME_LENGTH * maxGameLevels);
    for(int i = 0; i < maxGameLevels; i++) {
        allLevelData[i] = gameData->records[i].data;
        allLevelName[i] = gameData->records[i].name;
    }
}

void initializationGameLevel(int level) {
    initializationGameData(level);
}

void initializationGameResetLevel() {
    initializationGameLevel(currentLevel);
}

void initializationGameNextLevel() {
    currentLevel = (currentLevel >= maxGameLevels - 1) ? currentLevel : ++currentLevel;
    initializationGameLevel(currentLevel);
}

void initializationGamePreviousLevel() {
    currentLevel = (currentLevel <= 0) ? 0 : --currentLevel;
    initializationGameLevel(currentLevel);
}

void btnNumCommon(int position) {
    // 如果操作栈首元素是0，则表示开始操作
    if(opeartionStack[0] == 0) {
        toggleNumButton(FALSE);
        startOperation = TRUE;
        operandStack[0] = position;
        opeartionStack[0] = gameData[position];
    }
    else {
        toggleNumButton(TRUE);
        startOperation = FALSE;
        operandStack[1] = position;
        opeartionStack[2] = gameData[position];
        int result = 0;
        // 计算
        switch(opeartionStack[1]) {
            // plus
            case -1:
                result = opeartionStack[0] + opeartionStack[2];
                break;
            // minus
            case -2:
                result = opeartionStack[0] - opeartionStack[2];
                break;
            // multiply
            case -3:
                result = opeartionStack[0] * opeartionStack[2];
                break;
            // divide
            case -4:
                // 如果不能整除，不操作
                if(opeartionStack[0] % opeartionStack[2] != 0) {
                    // 重置 opeartionStack/operandStack
                    initOpeartionStack();
                    initOperandStack();
                    return;
                }
                // 如果可以整除，才操作
                else {
                    result = opeartionStack[0] / opeartionStack[2];
                    break;
                }
        }
        gameData[position] = result;
        currentCalcuationBtnPosition = position;
        // 隐藏第一个操作数按钮
        gtk_widget_hide(btnNums[operandStack[0]]);
        // 重置 opeartionStack/operandStack
        initOpeartionStack();
        initOperandStack();
        // 更新UI
        updateUI();
        checkGameOver();
    }
}

void btnOperationCommon(int position) {
    toggleNumButton(TRUE);
    // 加、减、乘、除 操作位置对应 -1, -2, -3, -4
    opeartionStack[1] = - position - 1;
}
