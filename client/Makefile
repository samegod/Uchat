NAME	=	uchat

CFLG	=	-std=c11 $(addprefix -W, all extra error pedantic) -g

GTK_LIBS = `pkg-config --cflags --libs gtk+-3.0`
GTK_INC = `pkg-config --cflags gtk+-3.0`

SRC_DIR	= src
INC_DIR	= inc
OBJ_DIR	= obj

INC_FILES = $(wildcard $(INC_DIR)/*.h)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:%.c=%.o)))

all: install

install: $(NAME)

$(NAME): $(OBJ_FILES)
	@clang $(CFLG) $(OBJ_FILES) $(GTK_LIBS) -lsqlite3 -o $@
	@printf "\r\33[2K$@ \033[32;1mcreated\033[0m\n"
	@mv $(NAME) ../$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@clang $(CFLG)  -c $< $(GTK_INC) -o $@ -I$(INC_DIR) -I$(LMX_INC)
	@printf "\r\33[2K$(NAME) \033[33;1mcompile \033[0m$(<:$(SRC_DIR)/%.c=%) "

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $@
	
clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(OBJ_DIR) in $(NAME) \033[31;1mdeleted\033[0m\n"

uninstall:
	@rm -rf $(OBJ_DIR)
	@rm -rf ../$(NAME)
	@rm -rf *.db
	@printf "$(NAME) \033[31;1muninstalled\033[0m\n"

reinstall: uninstall all
