#ifndef __COLORS_H__
#define __COLORS_H__

#define COLOR_YELLOW            "\x1b[33m"
#define RED_COLOR               "\033[0;31m"
#define RESET_COLOR             "\033[0m"

#define RED_MSG(msg)            (RED_COLOR msg RESET_COLOR)
#define YELLOW_MSG(msg)         (COLOR_YELLOW msg RESET_COLOR)

#endif //__COLORS_H__
