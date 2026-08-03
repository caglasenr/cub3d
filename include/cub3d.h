/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2026/08/03 14:12:32 by caglasener       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <stdbool.h>

# define WIN_W 1280
# define WIN_H 720

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef struct s_ray
{
    int map_x;
    int map_y;
    int step_x;
    int step_y;
    int side;
    double ray_dir_x;
    double ray_dir_y;
    double delta_dist_x;
    double delta_dist_y;
    double side_dist_x;
    double side_dist_y;
    double perp_wall_dist;
    double wall_x;
}	t_ray;
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
	char	*door_path;
	char	*sprite_path;
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
** t_gc: a simple linked-list allocator tracker. Every gc_malloc/gc_strdup
** call is registered here so a single gc_free_all_and_exit() call can
** release the whole parser's memory, even on an error exit.
*/
typedef struct s_gc
{
	void			*ptr;
	struct s_gc		*next;
}	t_gc;

/*
** t_map_line: temporary linked list the router appends map rows to while
** reading the file, before build_final_grid() turns it into cfg->map.
*/
typedef struct s_map_line
{
	char				*line;
	struct s_map_line	*next;
}	t_map_line;

/* ---- memory management (gc_utils.c) ---- */
void	*gc_malloc(t_gc **gc, size_t size);
char	*gc_strdup(t_gc **gc, const char *s1);
void	gc_free_all_and_exit(t_gc **gc, char *err_msg, int exit_code);

/* ---- entry point & routing (parser_router.c) ---- */
void	init_config(t_config *cfg);
int		check_extension(char *path);
void	check_textures(t_config *cfg, t_gc **gc);
bool	is_empty_line(char *line);
bool	is_config_line(char *line);
bool	is_map_line(char *line);
void	parse_cub_file(char *file_path, t_config *cfg, t_gc **gc);

/* ---- scene elements & colors (config_parser.c) ---- */
int		skip_whitespaces(char *line, int i);
bool	is_all_digits(char *str);
void	save_texture(char **target, char *line, int i, t_gc **gc);
void	parse_color(int *target, char *line, int i, t_gc **gc);
void	parse_config(char *line, t_config *cfg, t_gc **gc);

/* ---- map building (map_parser.c) ---- */
int		get_line_length(char *line);
void	append_map_line(char *line, t_map_line **list, t_gc **gc);
void	build_final_grid(t_config *cfg, t_map_line *list, t_gc **gc);

/* ---- validation (validator.c) ---- */
bool	flood_fill(t_config *cfg, int x, int y, char **copy);
void	validate_map(t_config *cfg, t_gc **gc);

/* ---- raycasting (src/raycasting/) ---- */
void	init_player(t_config *cfg, t_player *player);
void	cast_ray(t_player *player, t_config *cfg, double camera_x, t_ray *ray);
void	wall_height(t_ray *ray, int *draw_start, int *draw_end);

#endif
