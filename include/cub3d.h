/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2026/08/10 14:55:34 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define MOVE_SPEED 0.05
# define ROT_SPEED 0.03

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
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		side;
	double	ray_dir_x;
	double	ray_dir_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	double	perp_wall_dist;
	double	wall_x;
	char	hit_char;
	int		draw_start;
	int		draw_end;
}	t_ray;

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

typedef struct s_gc
{
	void			*ptr;
	struct s_gc		*next;
}	t_gc;

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

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_img		frame;
	t_img		textures[5];
	t_config	cfg;
	t_gc		*gc;
	t_player	player;
	int			keys[65536];
}	t_game;

typedef struct s_map_line
{
	char				*line;
	struct s_map_line	*next;
}	t_map_line;

typedef struct s_parse_ctx
{
	int			fd;
	t_config	*cfg;
	t_gc		**gc;
	bool		map_started;
	bool		map_ended;
	t_map_line	*map_list;
}	t_parse_ctx;

typedef struct s_cell
{
	int	x;
	int	y;
}	t_cell;

typedef struct s_flood_ctx
{
	char	**copy;
	t_cell	*stack;
	int		top;
	int		w;
	int		h;
	bool	leaked;
}	t_flood_ctx;

void	*gc_malloc(t_gc **gc, size_t size);
char	*gc_strdup(t_gc **gc, const char *s1);
void	gc_free_all_and_exit(t_gc **gc, char *err_msg, int exit_code);
void	init_config(t_config *cfg);
int		check_extension(char *path);
void	check_textures(t_config *cfg, t_gc **gc);
bool	is_empty_line(char *line);
bool	is_config_line(char *line);
bool	is_map_line(char *line);
void	parse_cub_file(char *file_path, t_config *cfg, t_gc **gc);
int		skip_whitespaces(char *line, int i);
bool	is_all_digits(char *str);
void	save_texture(char **target, char *line, int i, t_gc **gc);
void	parse_color(int *target, char *line, int i, t_gc **gc);
void	parse_config(char *line, t_config *cfg, t_gc **gc);
int		get_line_length(char *line);
void	append_map_line(char *line, t_map_line **list, t_gc **gc);
void	build_final_grid(t_config *cfg, t_map_line *list, t_gc **gc);
bool	flood_fill(t_config *cfg, int x, int y, char **copy);
void	validate_map(t_config *cfg, t_gc **gc);
void	init_player(t_config *cfg, t_player *player);
void	cast_ray(t_player *player, t_config *cfg, double camera_x, t_ray *ray);
void	wall_height(t_ray *ray);
void	init_game(t_game *game);
int		key_press(int keycode, t_game *game);
int		close_window(t_game *game);
void	put_pixel(t_img *img, int x, int y, int color);
int		tex_pixel(t_img *img, int x, int y);
double	get_tex_step(t_ray *ray, t_img *texture, int face, double *tex_pos);
void	draw_background(t_img *frame, t_config *config);
int		render_loop(t_game *game);
int		key_release(int keycode, t_game *game);
void	move_player(t_game *game);
void	toggle_door(t_game *game);
void	load_textures(t_game *game);
void	move_w(t_game *game);
void	move_s(t_game *game);
void	move_d(t_game *game);
void	move_a(t_game *game);
void	rotate_right(t_game *game);
void	rotate_left(t_game *game);

#endif
