BBLU		=	\033[1;34m
BGREEN		=	\033[1;32m
BRED		=	\033[1;31m
BLU			=	\033[0;34m
GRN			=	\033[0;32m
RED			=	\033[0;31m
RST			=	\033[0m

SRC_BOT		=	$(shell find ./bot -type f -name "*.cpp" | cut -c 7-)
HEADER_BOT	=	$(shell find ./bot -type f -name "*.hpp" | cut -c 3-)

SRC			=	$(shell find ./src -type f -name "*.cpp" | cut -c 7-)
HEADER		=	$(shell find ./inc -type f -name "*.hpp" | cut -c 3-)

OBJ			=	$(addprefix ./obj/, $(SRC:.cpp=.o))
OBJ_BOT		=	$(addprefix ./obj_bot/, $(SRC_BOT:.cpp=.o))

CXX			=	clang++
FLAGS		=	-Wall -Werror -Wextra -g3
FLAGS_H		=	-I inc/ -I bot/
NAME		=	ircserv

all: print_header $(NAME)
.PHONY: all

print_header:
	echo "$(BBLU)\033[5G========================================================================================================"
	echo "$(BBLU)\033[5G"
	echo "$(BBLU)\033[35G███████ ████████           ██ ██████   ██████ "
	echo "$(BBLU)\033[35G██         ██              ██ ██   ██ ██      "
	echo "$(BBLU)\033[35G█████      ██              ██ ██████  ██      "
	echo "$(BBLU)\033[35G██         ██              ██ ██   ██ ██      "
	echo "$(BBLU)\033[35G██         ██    ███████   ██ ██   ██  ██████ "
	echo "$(BBLU)\033[5G"
	echo "$(BBLU)\033[5G==================================> By kbarbry && tjolivea && mmosca <=================================="
.PHONY: print_header

./obj/%.o: ./src/%.cpp $(HEADER)
	mkdir -p obj obj/cmds
	$(CXX) $(FLAGS) $(FLAGS_H) -c $< -o $@
	echo "$(BBLU)[$(NAME) OBJ] :$(RST) $@ $(BGREEN)\033[47G[✔]$(RST)"

./obj_bot/%.o: ./bot/%.cpp $(HEADER_BOT)
	mkdir -p obj_bot
	$(CXX) $(FLAGS) $(FLAGS_H) -c $< -o $@
	echo "$(BBLU)[$(NAME) OBJ] :$(RST) $@ $(BGREEN)\033[47G[✔]$(RST)"

$(NAME): $(OBJ) $(OBJ_BOT)
	$(CXX) $(FLAGS) $(FLAGS_H) $(OBJ) -o $(NAME)
	echo "$(BGREEN)[$(NAME) END] :$(RST)$(RST) ./$(NAME) $(BGREEN)\033[47G[✔]$(RST)"
	$(CXX) $(FLAGS) $(FLAGS_H) $(OBJ_BOT) -o ircbot
	echo "$(BGREEN)[$(NAME) END] :$(RST)$(RST) ./ircbot $(BGREEN)\033[47G[✔]$(RST)"

clean:
	$(RM) $(OBJ) $(OBJ_BOT)
	echo "$(RED)[CLEAN]  :$(RST) Deleting objects...$(BGREEN)\033[47G[✔]$(RST)"
.PHONY: clean

fclean: clean
	$(RM) $(NAME) ./ircbot
	echo "$(RED)[FCLEAN] :$(RST) Deleting executable...$(BGREEN)\033[47G[✔]$(RST)"
.PHONY: fclean

re:	fclean
	$(MAKE) all
.PHONY: re

.SILENT:
