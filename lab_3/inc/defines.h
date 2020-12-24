#ifndef __DEFINES_H__
#define __DEFINES_H__

#define GET_RND_ELEM(percent)             ROLL_DICE(percent) ? 0 : (rand() % TOP_ELEM_LIMIT);

#define ROLL_DICE(percent)                ((rand() % 100) + 1) <= percent

#define COLOR_YELLOW            "\x1b[33m"
#define RED_COLOR               "\033[0;31m"
#define RESET_COLOR             "\033[0m"

#define RED_MSG(msg)            (RED_COLOR msg RESET_COLOR)
#define YELLOW_MSG(msg)         (COLOR_YELLOW msg RESET_COLOR)
         

typedef enum command_e
{
    EXIT_CMD,
    INPUT_MATRIX,
    GEN_RND_MATRIX,
    INPUT_VECTOR,
    GEN_RND_VECTOR,
    DEFAULT_MULT,
    SPARSE_MULT,
    PRINT_RES,
    CLEAN_SCREEN,
    PRINT_MATRIX,
    PRINT_VECTOR,
    UNKNOWN_CMD,
    CURRENT_MEM = 30
} command_t;

#endif // __DEFINES_H__
