/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 00:43:54 by jshi              #+#    #+#             */
/*   Updated: 2017/02/22 23:58:37 by jshi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int	setup_mlx(t_env *env)
{
	if (!(env->mlx = mlx_init()))
		return (0);
	if (!(env->win = mlx_new_window(env->mlx, WIN_LEN, WIN_WID, "Wolf3D")))
		return (0);
	if (!(env->img = mlx_new_image(env->mlx, WIN_LEN, WIN_WID)))
		return (0);
	env->data = mlx_get_data_addr(env->img, &env->bpp, &env->sl, &env->endian);
	env->bpp /= 8;
	if (!(env->skybox.img = mlx_xpm_file_to_image(env->mlx, "./img/skybox.xpm",
					&env->skybox.width, &env->skybox.height)))
		return (0);
	env->skybox.data = mlx_get_data_addr(env->skybox.img, &env->skybox.bpp,
			&env->skybox.sl, &env->skybox.endian);
	env->skybox.bpp /= 8;
	return (1);
}

static int	loop_hook(t_env *env)
{
	t_vec	next;

	if (env->dir == 1)
	{
		next.x = env->p.x + P_SPD * cos(env->ang_ver) * cos(env->ang_hor);
		next.y = env->p.y + P_SPD * cos(env->ang_ver) * sin(env->ang_hor);
		next.z = env->p.z + P_SPD * sin(env->ang_ver);
	}
	else if (env->dir == 2)
	{
		next.x = env->p.x - P_SPD * cos(env->ang_ver) * cos(env->ang_hor);
		next.y = env->p.y - P_SPD * cos(env->ang_ver) * sin(env->ang_hor);
		next.z = env->p.z - P_SPD * sin(env->ang_ver);
	}
	else if (env->dir == 3)
	{
		next.x = env->p.x + P_SPD * sin(env->ang_hor);
		next.y = env->p.y - P_SPD * cos(env->ang_hor);
		next.z = env->p.z;
	}
	else if (env->dir == 4)
	{
		next.x = env->p.x - P_SPD * sin(env->ang_hor);
		next.y = env->p.y + P_SPD * cos(env->ang_hor);
		next.z = env->p.z;
	}
	if (env->dir > 0 && next.x >= 0.0 && next.x < env->cols && next.y >= 0.0 &&
			next.y < env->rows && next.z >= 0.0 && next.z < env->flrs &&
			env->map[(int)next.z][(int)next.y][(int)next.x])
	{
		if (floor(next.x) == floor(env->p.x))
			env->p.x = next.x;
		if (floor(next.y) == floor(env->p.y))
			env->p.y = next.y;
		if (floor(next.z) == floor(env->p.z))
			env->p.z = next.z;
	}
	else if (env->dir > 0)
		env->p = next;
	draw_map(env);
	return (0);
}

static int	key_press_hook(int k, t_env *env)
{
	if (k == 13)
		env->dir = 1;
	else if (k == 1)
		env->dir = 2;
	else if (k == 0)
		env->dir = 3;
	else if (k == 2)
		env->dir = 4;
	return (0);
}

static int	key_release_hook(int k, t_env *env)
{
	if ((env->dir == 1 && k == 13) || (env->dir == 2 && k == 1) ||
			(env->dir == 3 && k == 0) || (env->dir == 4 && k == 2))
		env->dir = 0;
	if (k == 53)
		exit(0);
	return (0);
}

// camera rotation
// will fix later and add more features

static int	motion_hook(int x, int y, t_env *env)
{
	if (x >= 0 && x < WIN_LEN && y >= 0 && y < WIN_WID)
	{
		env->ang_hor = 3.0 * M_PI * ((double)x / WIN_LEN - 0.5);
		env->ang_ver = -2.0 * M_PI * ((double)y / WIN_WID - 0.5);
	}
	loop_hook(env);
	return (0);
}



static void	setup_hooks(t_env *env)
{
	mlx_key_hook(env->win, &key_release_hook, env);
	mlx_hook(env->win, 2, 0, &key_press_hook, env);
	mlx_hook(env->win, 6, 0, &motion_hook, env);
	mlx_loop_hook(env->mlx, &loop_hook, env);
	return ;
}

static void	init_vars(t_env *env)
{
	env->p.x = 0.0;
	env->ang_hor = 0.00000000001;
	env->ang_ver = 0.0;
	env->dir = 0;
}

int			main(int argc, char **argv)
{
	t_env	env;

	init_vars(&env);
	if (argc != 2)
	{
		ft_printf("Usage: ./wolf3d map_file\n");
		return (0);
	}
	if (!parse_map(&env, argv[1]))
	{
		ft_printf("Error: Invalid map file\n");
		return (0);
	}
	if (!setup_mlx(&env))
	{
		ft_printf("Error: mlx returned NULL\n");
		return (0);
	}
	draw_map(&env);
	setup_hooks(&env);
	mlx_loop(env.mlx);
	return (0);
}
