# **************************************************************************** #
#                                    cub3D                                     #
# **************************************************************************** #

NAME		= cub3D

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INC			= -Iinclude -Ilibft -Iget_next_line

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRC			= main.c \
			src/parser/parser_router.c \
			src/parser/config_parser.c \
			src/parser/map_parser.c \
			src/parser/validator.c \
			src/parser/gc_utils.c \
			get_next_line/get_next_line.c

OBJ			= $(SRC:.c=.o)

# ----------------------------------------------------------------------------
# MiniLibX (Linux). Raycaster'a başlarken:
#   1) minilibx-linux/ klasörünü proje köküne ekleyin
#   2) Aşağıdaki 3 satırı ve $(NAME) kuralındaki $(MLX) bağımlılığını + linki açın
#   3) MLX_FLAGS'i güncelleyin
# ----------------------------------------------------------------------------
# MLX_DIR	= minilibx-linux
# MLX		= $(MLX_DIR)/libmlx.a
# MLX_FLAGS	= -L$(MLX_DIR) -lmlx -lXext -lX11 -lm
MLX_FLAGS	= -lm

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX_FLAGS) -o $(NAME)

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
