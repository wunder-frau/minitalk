# ------------------------------ Sources ------------------------------

# Files
SERVER = server.c
CLIENT = client.c
LIBFT = cd libft_ && make
LIB = libft_/libft.a

# Sources and objects
SERVER_SRC = $(SERVER)
CLIENT_SRC = $(CLIENT)
SERVER_OBJS = $(SERVER_SRC:.c=.o)
CLIENT_OBJS = $(CLIENT_SRC:.c=.o)
OBJS = $(CLIENT_OBJS) $(SERVER_OBJS)

# ------------------------------ Constant strings ------------------------------

GCC = gcc
FLAGS = -Wall -Wextra -Werror
INCLUDE = -I include
SERVER_NAME = server
CLIENT_NAME = client

# ------------------------------ Colors ------------------------------

COLOR_GREEN = \033[32m
COLOR_PURPLE = \033[1;35m
RESET_COLOR = \033[0m

# ------------------------------ Messages ------------------------------

SERVER_MSG = echo "\n✅ $(COLOR_PURPLE)Server $(COLOR_GREEN)built successfully and ready to use! 🚀$(RESET_COLOR)"
CLIENT_MSG = echo "\n✅ $(COLOR_PURPLE)Client $(COLOR_GREEN)built successfully and ready to connect! 🌐$(RESET_COLOR)"

# ------------------------------ Rules ------------------------------

all: server client

libft_comp:
	@$(LIBFT)

server: $(SERVER_OBJS) libft_comp
	@$(GCC) $(FLAGS) $(SERVER_OBJS) $(LIB) -o $(SERVER_NAME)
	@$(SERVER_MSG)

client: $(CLIENT_OBJS) libft_comp
	@$(GCC) $(FLAGS) $(CLIENT_OBJS) $(LIB) -o $(CLIENT_NAME)
	@$(CLIENT_MSG)

clean:
	@rm -rf $(OBJS)
	@cd libft_ && make clean

fclean: clean
	@rm -rf $(SERVER_NAME) $(CLIENT_NAME)
	@cd libft_ && make fclean

re: fclean all

.c.o:
	@$(GCC) $(FLAGS) $(INCLUDE) -c $< -o $@

.PHONY: all clean fclean re server client libft_comp
