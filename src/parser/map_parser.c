#include "cub3d.h"

int	get_line_length(char *line)
{
	int	len;

	len = 0;
	while (line[len] && line[len] != '\n')
		len++;
	return (len);
}

static void	calculate_map_dimensions(t_map_line *list, t_config *cfg)
{
	t_map_line	*current;
	int			len;

	cfg->map_w = 0;
	cfg->map_h = 0;
	current = list;
	while (current != NULL)
	{
		len = get_line_length(current->line);
		if (len > cfg->map_w)
			cfg->map_w = len;
		cfg->map_h++;
		current = current->next;
	}
}

void	append_map_line(char *line, t_map_line **list, t_gc **gc)
{
	t_map_line	*new_node;
	t_map_line	*current;

	new_node = gc_malloc(gc, sizeof(t_map_line));
	new_node->line = gc_strdup(gc, line);
	new_node->next = NULL;
	if (*list == NULL)
		*list = new_node;
	else
	{
		current = *list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

void	build_final_grid(t_config *cfg, t_map_line *list, t_gc **gc)
{
	t_map_line	*curr;
	int			y;
	int			x;
	bool		player_found;

	calculate_map_dimensions(list, cfg);
	if (cfg->map_h == 0)
		gc_free_all_and_exit(gc, "map is missing", 1);
	cfg->map = gc_malloc(gc, sizeof(char *) * (cfg->map_h + 1));
	curr = list;
	y = 0;
	player_found = false;
	while (curr != NULL)
	{
		cfg->map[y] = gc_malloc(gc, sizeof(char) * (cfg->map_w + 1));
		x = 0;
		while (curr->line[x] && curr->line[x] != '\n')
		{
			if (!ft_strchr("01NSEWD ", curr->line[x]))
				gc_free_all_and_exit(gc, "invalid character in map", 1);
			cfg->map[y][x] = curr->line[x];
			if (ft_strchr("NSEW", curr->line[x]))
			{
				if (player_found)
					gc_free_all_and_exit(gc, "more than one player start in map", 1);
				cfg->player_x = x;
				cfg->player_y = y;
				cfg->player_dir = curr->line[x];
				player_found = true;
			}
			x++;
		}
		while (x < cfg->map_w)
			cfg->map[y][x++] = ' ';
		cfg->map[y][x] = '\0';
		y++;
		curr = curr->next;
	}
	cfg->map[y] = NULL;
	if (!player_found)
		gc_free_all_and_exit(gc, "map is missing a player start (N, S, E or W)", 1);
}
