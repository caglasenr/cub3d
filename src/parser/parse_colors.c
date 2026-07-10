/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Rejects a leading/trailing comma, an empty value, or two commas in a row,
** which ft_split would otherwise silently collapse.
*/
static int	has_bad_commas(char *s)
{
	char	*t;
	int		i;
	int		bad;

	t = ft_strtrim(s, " \t");
	if (!t)
		return (1);
	bad = 0;
	if (t[0] == ',' || t[0] == '\0')
		bad = 1;
	i = -1;
	while (t[++i])
		if (t[i] == ',' && (t[i + 1] == ',' || t[i + 1] == '\0'))
			bad = 1;
	free(t);
	return (bad);
}

static int	count_parts(char **parts)
{
	int	i;

	i = 0;
	while (parts[i])
		i++;
	return (i);
}

static int	all_digits(char *s)
{
	int	i;

	if (!s[0])
		return (0);
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	parse_channel(char *tok, int *out)
{
	char	*t;
	int		v;

	t = ft_strtrim(tok, " \t");
	if (!t)
		return (-1);
	if (!all_digits(t) || ft_strlen(t) > 3)
		return (free(t), -1);
	v = ft_atoi(t);
	free(t);
	if (v < 0 || v > 255)
		return (-1);
	*out = v;
	return (0);
}

int	parse_rgb(char *s, int *rgb)
{
	char	**parts;
	int		i;

	if (has_bad_commas(s))
		return (-1);
	parts = ft_split(s, ',');
	if (!parts)
		return (-1);
	if (count_parts(parts) != 3)
		return (free_split(parts), -1);
	i = -1;
	while (++i < 3)
		if (parse_channel(parts[i], &rgb[i]) != 0)
			return (free_split(parts), -1);
	free_split(parts);
	return (0);
}
