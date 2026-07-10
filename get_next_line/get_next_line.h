/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:27:37 by csener            #+#    #+#             */
/*   Updated: 2025/08/08 19:50:16 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

/*
** cub3D integration: this project links libft.a, which already provides
** ft_strchr / ft_strjoin / ft_strlen / ft_substr / ft_strdup. To avoid
** duplicate-symbol linker errors, get_next_line now reuses libft's versions
** (signatures are compatible) instead of redeclaring its own here.
** get_next_line_utils.c is therefore NOT compiled by the cub3D Makefile.
*/
# include "../libft/libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);

#endif