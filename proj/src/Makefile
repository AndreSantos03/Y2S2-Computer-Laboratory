# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/game/
.PATH: ${.CURDIR}/sprites/


# source code files to be compiled
SRCS = main.c graphics.c keyboard.c mouse.c timer.c utils.c game.c sprites.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
