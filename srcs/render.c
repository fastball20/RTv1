/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheiden <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 17:02:54 by maheiden          #+#    #+#             */
/*   Updated: 2019/02/14 19:22:30 by maheiden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/rtv.h"

void	set_pixel(SDL_Surface *surface, int x, int y, unsigned int pixel)
{
	*((unsigned int *)(surface->pixels + y * surface->pitch + x * 4)) = pixel;
}

void	clear_surface(SDL_Surface *surface)
{
	ft_bzero(surface->pixels, surface->h * surface->w * 4);
}

int		ft_lepri(int first, int second, double percent)
{
	if (first == second)
		return (first);
	return ((int)((double)first + (second - first) * percent));
}

int		get_color(int start, int end, double percent)
{
	int		r;
	int		g;
	int		b;

	if (start == end)
		return (start);
	r = ft_lepri((start >> 16) & 0xFF, (end >> 16) & 0xFF, percent);
	g = ft_lepri((start > 8) & 0xFF, (end >> 8) & 0xFF, percent);
	b = ft_lepri((start) & 0xFF, (end) & 0xFF, percent);
	return (r << 16 | g << 8 | b);
}

void	start_render(t_render *render)
{
	int 			i;
	int				t;
	double			z;
	t_intersection	bct;
	
	ray_cast(render);
	i = -1;
	while (++i < render->win_height * render->win_width)
	{
		t = -1;
		z = 1. / 0.;
		while (++t < render->triangle_nb)
		{
			bct = triangle_intersection(render->rays[i], render->triangle[t]);
			if (bct.z > 0 && bct.z < z)
			{
				z = bct.z;
				t_vector P = vector_sum(render->rays[i].origin, vector_scalar_multiply(render->rays[i].direction, z));
				t_vector N = cross_product(vector_sub(render->triangle[t].b, render->triangle[t].a), vector_sub(render->triangle[t].c, render->triangle[t].a));
				N = vector_normalize(N);
				if (dot_product(render->rays[i].direction, N) > 0)
					N = vector_scalar_multiply(N, -1);
				double dli = compute_lightning(render, P, N, (t_vector){0, 0, 0, 0});
				if (dli < 1)
					set_pixel(render->surface, i % render->win_width, i / render->win_width, get_color(0x0, 0xFFFF00, dli));
				else 
					set_pixel(render->surface, i % render->win_width, i / render->win_width, get_color(0xFFFF00, 0xFFFFFF, dli - 1));
			}
			t = -1;
			while (++t < render->sphere_nb)
			{
				double closest_t = sphere_intersection(render->rays[i], render->sphere[t]);
				if (closest_t > 0. && closest_t < z)
				{
					t_vector P = vector_sum(render->rays[i].origin, vector_scalar_multiply(render->rays[i].direction, closest_t));
					t_vector N = vector_sub(P, render->sphere[t].center);
					N = vector_normalize(N);
					t_vector V = vector_scalar_multiply(render->rays[i].direction, -1);
				//	V = vector_scalar_multiply(render->rays[i].direction, closest_t);
					double dli = compute_lightning(render, P, N, V);
	//					if (dli > 2)
	//						dli = 2;
					if (dli < 1)
						set_pixel(render->surface, i % render->win_width, i / render->win_width, get_color(0x0, 0xFF0000, dli));
					else 
						set_pixel(render->surface, i % render->win_width, i / render->win_width, get_color(0xFF0000, 0xFFFFFF, dli - 1));
				}
			}
			t = -1;
			while (++t < render->cylinder_nb)
			{
				double closest_c = cylinder_intersection(render->rays[i], render->cylinder[t]);
				if (closest_c > 0. && closest_c < z)
				{
					t_vector P = vector_sum(render->rays[i].origin, vector_scalar_multiply(render->rays[i].direction, closest_c));
					t_vector N = vector_sub(P, render->sphere[t].center);
					N = vector_normalize(N);
					t_vector V = vector_scalar_multiply(render->rays[i].direction, -1);
					double dli = compute_lightning(render, P, N, V);
						//if (dli > 2)
	//						dli = 2;
					if (dli < 1)
						set_pixel(render->surface, i % render->win_width, i / render->win_width, get_color(0x0, 0xFF0000, dli));
					else 
						set_pixel(render->surface, i % render->win_width, i / render->win_width, get_color(0xFF0000, 0xFFFFFF, dli - 1));
				}
			}
		}
	}
	SDL_UpdateWindowSurface(render->window);
}
