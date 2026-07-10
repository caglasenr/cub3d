/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

/*
** t_config: the parser's output and the data contract with the raycaster.
** floor/ceiling hold R,G,B; each channel is -1 until the element is parsed.
** map keeps every row exactly as it appears in the file (spaces preserved).
*/
typedef struct s_config
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		floor[3];
	int		ceiling[3];
	int		has_floor;
	int		has_ceil;
	char	**map;
	int		map_h;
	int		map_w;
	int		player_x;
	int		player_y;
	char	player_dir;
}	t_config;

/*
** t_img: a ready-to-use MLX image wrapper for the raycaster (frame + textures).
** The parser does not touch it; it is here so the whole team shares one header.
*/
typedef struct s_img
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

/*
** t_fill: internal helper for the flood-fill closed-map check.
*/
typedef struct s_fill
{
	int	h;
	int	w;
	int	leak;
}	t_fill;

/* ---- parser entry point (parser.c) ---- */
int		parse_scene(t_config *cfg, char *path);
void	init_config(t_config *cfg);
char	**read_file(int fd);
int		parse_lines(t_config *cfg, char **lines, int *map_start);
int		build_map(t_config *cfg, char **lines, int start);

/* ---- elements (parse_elements.c) ---- */
int		parse_element_line(t_config *cfg, char *line);
int		set_texture(char **dst, char *rest);
int		set_color(int *rgb, int *has, char *rest);
int		check_textures(t_config *cfg);

/* ---- colors (parse_colors.c) ---- */
int		parse_rgb(char *s, int *rgb);

/* ---- map (parse_map.c) ---- */
int		collect_map(t_config *cfg, char **lines, int start);
int		scan_map(t_config *cfg);

/* ---- validation (validate_map.c) ---- */
int		check_closed(t_config *cfg);

/* ---- utils (parser_utils.c) ---- */
int		err(char *msg);
int		check_extension(char *path);
int		is_space(char c);
int		is_empty_line(char *line);
void	free_split(char **arr);
void	free_config(t_config *cfg);

#endif
