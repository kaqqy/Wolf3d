/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_inter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 23:19:31 by jshi              #+#    #+#             */
/*   Updated: 2017/02/21 00:50:10 by jshi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static double	dist_sq(t_vec *a, t_vec *b)
{
	return ((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y) +
			(a->z - b->z) * (a->z - b->z));
}

static t_pix	find_inter_x(t_env *env, t_vec *dir, t_vec *pt)
{
	double	offs;
	double	dist;
	t_vec	inter;

	if (dir->x == 0.0)
		return (NULL_PIX);
	offs = (dir->x > 0) ? 0.0 : -1.0;
	inter.x = (dir->x > 0) ? ceil(pt->x) : floor(pt->x);
	inter.x = (dir->x > 0) ? MAX(inter.x, 0) : MIN(inter.x, env->cols);
	while (inter.x + offs >= 0.0 && inter.x + offs < env->cols)
	{
		dist = inter.x - pt->x;
		inter.y = pt->y + dir->y * dist / dir->x;
		inter.z = pt->z + dir->z * dist / dir->x;
		if (inter.y >= 0.0 && inter.y < env->rows &&
				inter.z >= 0.0 && inter.z < env->flrs &&
				env->map[(int)inter.z][(int)inter.y][(int)(inter.x + offs)])
			return ((t_pix){0xff0000, dist_sq(pt, &inter)});
		inter.x += 2 * offs + 1.0;
	}
	return (NULL_PIX);
}

static t_pix	find_inter_y(t_env *env, t_vec *dir, t_vec *pt)
{
	double	offs;
	double	dist;
	t_vec	inter;

	if (dir->y == 0.0)
		return (NULL_PIX);
	offs = (dir->y > 0) ? 0.0 : -1.0;
	inter.y = (dir->y > 0) ? ceil(pt->y) : floor(pt->y);
	inter.y = (dir->y > 0) ? MAX(inter.y, 0) : MIN(inter.y, env->rows);
	while (inter.y + offs >= 0.0 && inter.y + offs < env->rows)
	{
		dist = inter.y - pt->y;
		inter.x = pt->x + dir->x * dist / dir->y;
		inter.z = pt->z + dir->z * dist / dir->y;
		if (inter.x >= 0.0 && inter.x < env->cols &&
				inter.z >= 0.0 && inter.z < env->flrs &&
				env->map[(int)inter.z][(int)(inter.y + offs)][(int)inter.x])
			return ((t_pix){0x00ff00, dist_sq(pt, &inter)});
		inter.y += 2 * offs + 1.0;
	}
	return (NULL_PIX);
}

static t_pix	find_inter_z(t_env *env, t_vec *dir, t_vec *pt)
{
	double	offs;
	double	dist;
	t_vec	inter;

	if (dir->z == 0.0)
		return (NULL_PIX);
	offs = (dir->z > 0) ? 0.0 : -1.0;
	inter.z = (dir->z > 0) ? ceil(pt->z) : floor(pt->z);
	inter.z = (dir->z > 0) ? MAX(inter.z, 0) : MIN(inter.z, env->flrs);
	while (inter.z + offs >= 0.0 && inter.z + offs < env->flrs)
	{
		dist = inter.z - pt->z;
		inter.x = pt->x + dir->x * dist / dir->z;
		inter.y = pt->y + dir->y * dist / dir->z;
		if (inter.x >= 0.0 && inter.x < env->cols &&
				inter.y >= 0.0 && inter.y < env->rows &&
				env->map[(int)(inter.z + offs)][(int)inter.y][(int)inter.x])
			return ((t_pix){0x0000ff, dist_sq(pt, &inter)});
		inter.z += 2 * offs + 1.0;
	}
	return (NULL_PIX);
}

int				find_inter(t_env *env, t_vec *dir, t_vec *pt)
{
	t_pix	x;
	t_pix	y;
	t_pix	z;

	(void)pt;
	x = find_inter_x(env, dir, &env->p);
	y = find_inter_y(env, dir, &env->p);
	z = find_inter_z(env, dir, &env->p);
	if (x.dist < y.dist && x.dist < z.dist)
		return (x.color);
	if (y.dist < z.dist)
		return (y.color);
	return (z.color);
}
