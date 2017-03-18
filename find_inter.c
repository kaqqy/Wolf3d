/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_inter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 23:19:31 by jshi              #+#    #+#             */
/*   Updated: 2017/02/23 00:13:38 by jshi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static double	dist_sq(t_vec *a, t_vec *b)
{
	return ((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y) +
			(a->z - b->z) * (a->z - b->z));
}

/*
** Move to separate file
*/

static int		get_point(t_img *img, int x, int y)
{
	int		ind;

	ind = img->sl * y + img->bpp * x;
	return (img->data[ind] + (img->data[ind + 1] << 8) + (img->data[ind + 2] << 16));
}

static int		get_color(t_img *img, t_vec *dir)
{
	t_vec	abs_dir;
	int		x;
	int		y;

	abs_dir = (t_vec){fabs(dir->x), fabs(dir->y), fabs(dir->z)};
	if (abs_dir.x > abs_dir.y && abs_dir.x > abs_dir.z)
	{
		x = (int)((1 + dir->y / dir->x) * 512);
		y = (int)((1 - dir->z / dir->x) * 512);
		if (x < 0)
			x = 0;
		if (x >= 1024)
			x = 1023;
		if (y < 0)
			y = 0;
		if (y >= 1024)
			y = 1023;
		if (dir->x > 0.0)
			return (get_point(img, x, y + 1024));
		return (get_point(img, x + 2048, 2047 - y));
	}
	else if (abs_dir.y > abs_dir.z)
	{
		x = (int)((1 - dir->x / dir->y) * 512);
		y = (int)((1 - dir->z / dir->y) * 512);
		if (x < 0)
			x = 0;
		if (x >= 1024)
			x = 1023;
		if (y < 0)
			y = 0;
		if (y >= 1024)
			y = 1023;
		if (dir->y > 0.0)
			return (get_point(img, x + 1024, y + 1024));
		return (get_point(img, x + 3072, 2047 - y));
	}
	x = (int)((1 - dir->x / dir->z) * 512);
	y = (int)((1 + dir->y / dir->z) * 512);
	if (x < 0)
		x = 0;
	if (x >= 1024)
		x = 1023;
	if (y < 0)
		y = 0;
	if (y >= 1024)
		y = 1023;
	if (dir->z > 0.0)
		return (get_point(img, x + 1024, y));
	return (get_point(img, 2047 - x, y + 2048));
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
			return ((t_pix){(offs == 0.0) ? 0xff0000 : 0xffff00, dist_sq(pt, &inter)});
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
			return ((t_pix){(offs == 0.0) ? 0x00ff00 : 0x00ffff, dist_sq(pt, &inter)});
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
			return ((t_pix){(offs == 0.0) ? 0x0000ff : 0xff00ff, dist_sq(pt, &inter)});
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
	if (z.dist != DBL_MAX)
		return (z.color);
	return (get_color(&env->skybox, dir));
}
