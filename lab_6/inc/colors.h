#ifndef __COLORS_H__
#define __COLORS_H__

#define RED_COLOR               "\033[31m"
#define GREEN_COLOR             "\033[32m"
#define YELLOW_COLOR            "\x1b[33m"
#define BLUE_COLOR              "\033[34m"
#define MAGENTA_COLOR           "\033[35m"
#define RESET_COLOR             "\033[0m"

#define RED_MSG(msg)            (RED_COLOR msg RESET_COLOR)
#define YELLOW_MSG(msg)         (COLOR_YELLOW msg RESET_COLOR)

#endif //__COLORS_H__
