#include "cub3d.h"

void	init_config(t_config *cfg)
{
	ft_memset(cfg, 0, sizeof(t_config));
}

int	check_extension(char *path)
{
	size_t	len;

	len = ft_strlen(path);
	if (len < 5)
		return (-1);
	if (ft_strncmp(path + len - 4, ".cub", 4) != 0)
		return (-1);
	return (0);
}

void	check_textures(t_config *cfg, t_gc **gc)
{
	char	*paths[6];
	int		i;
	int		fd;

	paths[0] = cfg->no_path;
	paths[1] = cfg->so_path;
	paths[2] = cfg->we_path;
	paths[3] = cfg->ea_path;
	paths[4] = cfg->door_path;
	paths[5] = cfg->sprite_path;
	i = -1;
	while (++i < 6)
	{
		if (!paths[i])
			continue ;
		fd = open(paths[i], O_RDONLY);
		if (fd < 0)
			gc_free_all_and_exit(gc, "cannot open a texture file", 1);
		close(fd);
	}
}

bool	is_empty_line(char *line)
{
	int	i;

	if (!line)
		return (true);
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (false);
		i++;
	}
	return (true);
}

bool	is_config_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (!line[i])
		return (false);
	if (ft_strncmp(line + i, "NO ", 3) == 0 || ft_strncmp(line + i, "SO ", 3) == 0
		|| ft_strncmp(line + i, "WE ", 3) == 0 || ft_strncmp(line + i, "EA ", 3) == 0
		|| ft_strncmp(line + i, "DO ", 3) == 0 || ft_strncmp(line + i, "SP ", 3) == 0)
		return (true);
	if ((line[i] == 'F' || line[i] == 'C') && (line[i + 1] == ' ' || line[i + 1] == '\t'))
		return (true);
	return (false);
}

bool	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	return (line[i] == '1' || line[i] == '0' || line[i] == 'N' || line[i] == 'S'
		|| line[i] == 'E' || line[i] == 'W' || line[i] == 'D');
}

void	parse_cub_file(char *file_path, t_config *cfg, t_gc **gc)
{
	int			fd;
	char		*line;
	bool		map_started;
	t_map_line	*map_list;

	map_started = false;
	map_list = NULL;
	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		gc_free_all_and_exit(gc, "cannot open the map file", 1);
	line = get_next_line(fd);
	while (line)
	{
		if (is_empty_line(line))
			free(line);
		else if (is_config_line(line))
		{
			if (map_started)
			{
				free(line);
				close(fd);
				gc_free_all_and_exit(gc, "configuration item after map has started", 1);
			}
			parse_config(line, cfg, gc);
			free(line);
		}
		else if (is_map_line(line))
		{
			map_started = true;
			append_map_line(line, &map_list, gc);
			free(line);
		}
		else
		{
			free(line);
			close(fd);
			gc_free_all_and_exit(gc, "unrecognized line in map file", 1);
		}
		line = get_next_line(fd);
	}
	close(fd);
	if (!cfg->no_path || !cfg->so_path || !cfg->we_path || !cfg->ea_path
		|| !cfg->has_floor || !cfg->has_ceil)
		gc_free_all_and_exit(gc, "missing or incomplete scene element", 1);
	build_final_grid(cfg, map_list, gc);
	validate_map(cfg, gc);
	check_textures(cfg, gc);
}
