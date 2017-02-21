/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 19:42:08 by jshi              #+#    #+#             */
/*   Updated: 2017/02/21 01:07:24 by jshi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	draw_point(t_env *env, int x, int y, int color)
{
	int		ind;

	ind = env->sl * y + env->bpp * x;
	if (env->endian)
	{
		env->data[ind + env->bpp - 1] = color % 256;
		env->data[ind + env->bpp - 2] = (color >> 8) % 256;
		env->data[ind + env->bpp - 3] = (color >> 16) % 256;
	}
	else
	{
		env->data[ind] = color % 256;
		env->data[ind + 1] = (color >> 8) % 256;
		env->data[ind + 2] = (color >> 16) % 256;
	}
}

static void	rotate_vec(t_env *env, t_vec *d)
{
	double	vec_mag;
	double	ang_ver;
	t_vec	ver;
	t_vec	hor;

	vec_mag = sqrt(d->x * d->x + d->z * d->z);
	ang_ver = env->ang_ver + atan2(d->z, d->x);
	ver.x = vec_mag * cos(env->ang_hor) * cos(ang_ver);
	ver.y = vec_mag * sin(env->ang_hor) * cos(ang_ver);
	ver.z = vec_mag * sin(ang_ver);
	hor = (t_vec){d->y * -sin(env->ang_hor), d->y * cos(env->ang_hor), 0};
	d->x = ver.x + hor.x;
	d->y = ver.y + hor.y;
	d->z = ver.z;
}

/*
** Current FOV angle is 90 degrees
** If angle is t degrees
** dir = (t_vec){MIN_DIST_WALL * cos(t / 2),
**         (2.0 * j / (WIN_LEN - 1) - 1.0) * MIN_DIST_WALL * sin(t / 2),
**         (2.0 * i / (WIN_WID - 1) - 1.0) * MIN_DIST_WALL * sin(t / 2)}
*/

void		draw_partial(t_thrd *thread)
{
	t_env	*env;
	int		i;
	int		j;
	t_vec	dir;
	t_vec	pt;

	env = thread->env;
	i = -1;
	while (++i < WIN_WID)
	{
		j = -1;
		while (++j < WIN_LEN)
		{
			if ((i * WIN_LEN + j) % NUM_THREADS != thread->i)
				continue;
			dir = (t_vec){MIN_DIST_WALL / sqrt(2),
				((double)j / (WIN_LEN - 1) - 0.5) * MIN_DIST_WALL * sqrt(2),
				(0.5 - (double)i / (WIN_WID - 1)) * MIN_DIST_WALL * sqrt(2)};
			rotate_vec(env, &dir);
			pt = (t_vec){dir.x + env->p.x, dir.y + env->p.y, dir.z + env->p.z};
			draw_point(env, j, i, find_inter(env, &dir, &pt));
		}
	}
}

void		draw_map(t_env *env)
{
	int		i;
	t_thrd	*thread;

	i = -1;
	while (++i < NUM_THREADS)
	{
		thread = (t_thrd*)malloc(sizeof(*thread));
		thread->i = i;
		thread->env = env;
		pthread_create(&env->threads[i], NULL, (void*)draw_partial, thread);
	}
	i = -1;
	while (++i < NUM_THREADS)
		pthread_join(env->threads[i], NULL);
	mlx_clear_window(env->mlx, env->win);
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
}
