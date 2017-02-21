/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 15:34:14 by jshi              #+#    #+#             */
/*   Updated: 2017/02/21 01:03:21 by jshi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include <float.h>
# include <pthread.h>
# include "mlx.h"
# include "libft.h"
# include "ft_printf.h"

# define WIN_LEN 960
# define WIN_WID 720
# define MIN_DIST_WALL 0.1
# define NULL_PIX (t_pix){0, DBL_MAX}
# define NUM_THREADS 16

typedef struct	s_vec
{
	double		x;
	double		y;
	double		z;
}				t_vec;

typedef struct	s_pix
{
	int			color;
	double		dist;
}				t_pix;

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
	t_vec		p;
	double		ang_hor;
	double		ang_ver;
	pthread_t	threads[NUM_THREADS];
}				t_env;

typedef struct	s_thrd
{
	int			i;
	t_env		*env;
}				t_thrd;

int				parse_map(t_env *env, char *fn);
void			draw_map(t_env *env);
int				find_inter(t_env *env, t_vec *dir, t_vec *pt);
#endif
