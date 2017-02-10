/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 00:43:54 by jshi              #+#    #+#             */
/*   Updated: 2017/02/08 19:41:27 by jshi             ###   ########.fr       */
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

static void	setup_hooks(t_env *env)
{
	// to do
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
