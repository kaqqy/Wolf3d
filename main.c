/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 00:43:54 by jshi              #+#    #+#             */
/*   Updated: 2017/02/21 00:52:51 by jshi             ###   ########.fr       */
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
	return (1);
}

static int	key_release_hook(int k, t_env *env)
{
	if (k == 13)
	{
		env->p.x += 0.2 * cos(env->ang_ver) * cos(env->ang_hor);
		env->p.y += 0.2 * cos(env->ang_ver) * sin(env->ang_hor);
		env->p.z += 0.2 * sin(env->ang_ver);
		draw_map(env);
	}
	else if (k == 1)
	{
		env->p.x -= 0.2 * cos(env->ang_ver) * cos(env->ang_hor);
		env->p.y -= 0.2 * cos(env->ang_ver) * sin(env->ang_hor);
		env->p.z -= 0.2 * sin(env->ang_ver);
		draw_map(env);
	}
	return (0);
}

// camera rotation
// will fix later and add more features

static int	motion_hook(int x, int y, t_env *env)
{
	if (x >= 0 && x < WIN_LEN && y >= 0 && y < WIN_WID)
	{
		env->ang_hor = 3.0 * M_PI * ((double)x / WIN_LEN - 0.5);
		env->ang_ver = -3.0 * M_PI * ((double)y / WIN_WID - 0.5);
	}
	draw_map(env);
	return (0);
}

static void	setup_hooks(t_env *env)
{
	mlx_key_hook(env->win, &key_release_hook, env);
	mlx_hook(env->win, 6, 0, &motion_hook, env);
	return ;
}

int			main(int argc, char **argv)
{
	t_env	env;

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
