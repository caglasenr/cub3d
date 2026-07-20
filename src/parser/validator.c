#include "cub3d.h"

bool	flood_fill(t_config *cfg, int x, int y, char **copy)
{
	bool	top;
	bool	bottom;
	bool	left;
	bool	right;

	if (x < 0 || x >= cfg->map_w || y < 0 || y >= cfg->map_h)
		return (false);
	if (copy[y][x] == ' ')
		return (false);
	if (copy[y][x] == '1' || copy[y][x] == 'V')
		return (true);
	copy[y][x] = 'V';
	top = flood_fill(cfg, x, y - 1, copy);
	bottom = flood_fill(cfg, x, y + 1, copy);
	left = flood_fill(cfg, x - 1, y, copy);
	right = flood_fill(cfg, x + 1, y, copy);
	return (top && bottom && left && right);
}

void	validate_map(t_config *cfg, t_gc **gc)
{
	char	**map_copy;
	int		y;

	map_copy = gc_malloc(gc, sizeof(char *) * (cfg->map_h + 1));
	y = 0;
	while (y < cfg->map_h)
	{
		map_copy[y] = gc_strdup(gc, cfg->map[y]);
		y++;
	}
	map_copy[y] = NULL;
	if (!flood_fill(cfg, cfg->player_x, cfg->player_y, map_copy))
		gc_free_all_and_exit(gc, "map is not closed, there are gaps in the walls", 1);
}
