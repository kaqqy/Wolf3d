/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 15:45:36 by jshi              #+#    #+#             */
/*   Updated: 2017/02/08 17:38:50 by jshi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int	parse_dimensions(t_env *env, int fd)
{
	char	*line;
	char	**ss;

	if (get_next_line(fd, &line) <= 0)
		return (0);
	if (!(ss = ft_strsplit(line, ' ')) || ft_strarrlen(ss) != 3)
		return (0);
	if (!ft_is_int(ss[0]) || !ft_is_int(ss[1]) || !ft_is_int(ss[2]))
		return (0);
	if ((env->rows = ft_atoi(ss[0])) <= 0 ||
			(env->cols = ft_atoi(ss[1])) <= 0 ||
			(env->flrs = ft_atoi(ss[2])) <= 0)
		return (0);
	free(line);
	ft_strarrdel(&ss);
	return (1);
}

static int	alloc_map(t_env *env)
{
	int		i;
	int		j;

	if (!(env->map = (int***)malloc(sizeof(*env->map) * env->flrs)))
		return (0);
	i = -1;
	while (++i < env->flrs)
		if (!(env->map[i] = (int**)malloc(sizeof(**env->map) * env->rows)))
			return (0);
	i = -1;
	while (++i < env->flrs && (j = -1))
		while (++j < env->rows)
			if (!(env->map[i][j] =
						(int*)malloc(sizeof(***env->map) * env->cols)))
				return (0);
	return (1);
}

static void	parse_line(t_env *env, char **ss, int i, int j)
{
	int		k;

	k = -1;
	while (++k < env->cols)
	{
		env->map[i][j][k] = ft_atoi(ss[k]);
		if (ss[k][0] == 'p')
		{
			env->px = k + 0.5;
			env->py = j + 0.5;
			env->pz = i + 0.5;
			env->ang_hor = 0;
			env->ang_ver = 0;
		}
	}
}

static int	parse_file(t_env *env, int fd)
{
	int		i;
	int		j;
	char	*line;
	char	**ss;

	i = -1;
	while (++i < env->flrs)
	{
		j = -1;
		while (++j < env->rows)
		{
			if (get_next_line(fd, &line) <= 0)
				return (0);
			if (!(ss = ft_strsplit(line, ' ')) || ft_strarrlen(ss) != env->cols)
				return (0);
			free(line);
			parse_line(env, ss, i, j);
			ft_strarrdel(&ss);
		}
	}
	if (get_next_line(fd, &line) != 0)
		return (0);
	return (1);
}

int		parse_map(t_env *env, char *fn)
{
	int		fd;

	if ((fd = open(fn, O_RDONLY)) < 0)
		return (0);
	if (!parse_dimensions(env, fd))
		return (0);
	if (!alloc_map(env))
		return (0);
	if (!parse_file(env, fd))
		return (0);
	return (1);
}
