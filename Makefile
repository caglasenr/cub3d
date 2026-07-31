# **************************************************************************** #
#                                                                              #
#                                    cub3D                                     #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -Iinclude -Ilibft -Iget_next_line -I$(MLX_DIR)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c \
      src/parser/parser_router.c \
      src/parser/config_parser.c \
      src/parser/map_parser.c \
      src/parser/validator.c \
      src/parser/gc_utils.c \
      src/raycasting/events.c \
      src/raycasting/game_init.c \
      src/raycasting/player_init.c \
      src/raycasting/render.c \
      get_next_line/get_next_line.c
OBJ = $(SRC:.c=.o)

# ---------------------------------------------------------------------------- #
# MiniLibX (Linux). Raycaster'a başlarken:
# ---------------------------------------------------------------------------- #
MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(LIBFT) $(MLX) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX) $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJ)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus