/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 15:34:14 by jshi              #+#    #+#             */
/*   Updated: 2017/02/09 02:12:44 by jshi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H
# define WIN_LEN 1280
# define WIN_WID 960
# define MIN_DIST_WALL 0.1

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include "mlx.h"
# include "libft.h"
# include "ft_printf.h"

typedef struct	s_vec
{
	double		x;
	double		y;
	double		z;
}				t_vec;

typedef struct	s_env
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*data;
	int			bpp;
	int			sl;
	int			endian;
	int			rows;
	int			cols;
	int			flrs;
	int			***map;
	double		px;
	double		py;
	double		pz;
	double		ang_hor;
	double		ang_ver;
}				t_env;

int			parse_map(t_env *env, char *fn);
void		draw_map(t_env *env);
#endif
