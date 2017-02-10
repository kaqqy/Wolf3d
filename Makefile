# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jshi <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/26 23:47:37 by jshi              #+#    #+#              #
#    Updated: 2017/02/08 19:42:02 by jshi             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d

FILES = main parse_map draw_map
SRC_FILES = $(addsuffix .c,$(FILES))
OBJ_FILES = $(addsuffix .o,$(FILES))

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a
LIBFT_LINK = -L$(LIBFT_PATH)
LIBFT_HDR = -I$(LIBFT_PATH)/includes

MLX_PATH = ./minilibx
MLX = $(MLX_PATH)/libmlx.a
MLX_LINK = -L$(MLX_PATH)
MLX_HDR = -I$(MLX_PATH)

HDR_PATH = .
HDR = -I$(HDR_PATH)

CFLAGS = -Wall -Wextra -Werror


.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT) $(MLX)
	gcc -o $(NAME) $(OBJ_FILES) $(LIBFT_LINK) -lft $(MLX_LINK) -lmlx \
		-framework OpenGL -framework AppKit

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@ $(HDR) $(LIBFT_HDR) $(MLX_HDR)

$(LIBFT):
	make -C $(LIBFT_PATH)

$(MLX):
	make -C $(MLX_PATH)

clean:
	rm -f $(OBJ_FILES)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean
	make -C $(MLX_PATH) clean

re: fclean all
