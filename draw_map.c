/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jshi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 19:42:08 by jshi              #+#    #+#             */
/*   Updated: 2017/02/09 02:14:53 by jshi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		draw_map(t_env *env)
{
	// 0.1 = furthest dist from player to projection plane = MIN_DIST_WALL
	// for (int i = 0; i < WIN_WID; i++)
	// for (int j = 0; j < WIN_LEN; j++)
	// point = ((double)j / (WIN_LEN - 1) - 0.5) * 0.1,
	// ((double)i / (WIN_WID - 1) - 0.5) * 0.1
	// vector = point.x + 0.05 * sqrt3, point.y
	// rotate vector by ang_hor horizontally
	// rotate vector by ang_ver vertically
	// find intersection from vector to walls in all three direction
}
