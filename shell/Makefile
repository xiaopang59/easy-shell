CPPFLAGS = -I ./include
CFLAGS = -g -Wall
LDFLAGS = -L ./lib -lmycalc
CC = gcc

D_SRC = ./src
D_OBJ = ./obj

SRC_C	= $(wildcard $(D_SRC)/*.c)
OBJ_C	= $(addprefix $(D_OBJ)/, $(patsubst %.c, %.o, $(notdir $(SRC_C))))

TARGET = myshell

$(TARGET): $(OBJ_C)
	$(CC) $^ -o $@

$(D_OBJ)/%.o: $(D_SRC)/%.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

.PHONY: clean
# 彻底清除生成过程文件
clean:
	-rm -f $(D_OBJ)/*.o
	-rm -f myshell
