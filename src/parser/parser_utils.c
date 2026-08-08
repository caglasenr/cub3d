/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:58:21 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/07 15:58:21 by caglasener       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	skip_whitespaces(char *line, int i)
{
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	return (i);
}

bool	is_all_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i]) && str[i] != '\n'
			&& str[i] != ' ')
			return (false);
		i++;
	}
	return (true);
}

// Satır sadece boşluk/tab/newline'dan mı oluşuyor, yoksa tamamen boş mu?
// (parse_cub_file boş satırları atlamak için kullanıyor)
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

// Satır bir config satırı mı (NO/SO/WE/EA/DO/SP dokusu ya da F/C renk tanımı)?
bool	is_config_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (!line[i])
		return (false);
	if (ft_strncmp(line + i, "NO ", 3) == 0
		|| ft_strncmp(line + i, "SO ", 3) == 0
		|| ft_strncmp(line + i, "WE ", 3) == 0
		|| ft_strncmp(line + i, "EA ", 3) == 0
		|| ft_strncmp(line + i, "DO ", 3) == 0
		|| ft_strncmp(line + i, "SP ", 3) == 0)
		return (true);
	if ((line[i] == 'F' || line[i] == 'C')
		&& (line[i + 1] == ' ' || line[i + 1] == '\t'))
		return (true);
	return (false);
}

// Satır harita satırı mı (boşluklardan sonra 0/1/N/S/E/W/D ile başlıyor mu)?
bool	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	return (line[i] == '1' || line[i] == '0' || line[i] == 'N' || line[i] == 'S'
		|| line[i] == 'E' || line[i] == 'W' || line[i] == 'D');
}
