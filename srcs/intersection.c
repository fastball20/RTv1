/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_intersetction.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheiden <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 15:16:01 by maheiden          #+#    #+#             */
/*   Updated: 2019/02/09 21:53:23 by maheiden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/rtv.h"

double	sphere_intersection(t_ray ray, t_sphere sphere)
{
	t_vector 		oc;

	oc = vector_sub(ray.origin, sphere.center);
	double k1 = dot_product(ray.direction, ray.direction);
	double k2 = 2 * dot_product(oc, ray.direction);
	double k3 = dot_product(oc, oc) - vector_length(sphere.r) *
	   	vector_length(sphere.r);
	double diskr = k2 * k2 - 4 * k1 * k3;
	if (diskr < 0)
		return (0);
	double t1 = (-k2 + sqrt(diskr)) / (2 * k1);
	double t2 = (-k2 + sqrt(diskr)) / (2 * k1);
	if (t1 > t2)
	{
		double tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 < 0)
		t1 = t2;
	if (t1 < 0)
		return (-1.);
	return (t1);
}

t_intersection			triangle_intersection(t_ray ray, t_triangle triangle)
{
	t_vector			e1;
	t_vector			e2;
	t_vector			pvec;
	t_vector			tvec;
	t_vector			qvec;
	t_intersection		res;
	double				det;


	res = (t_intersection){0, 0, 0};
	e1 = vector_sub(triangle.a, triangle.b);
	e2 = vector_sub(triangle.c, triangle.a);
	pvec = cross_product(ray.direction, e2);
	det = dot_product(e1, pvec);
	if (det < 1e-8 && det > -1e-8)
		return (res);
	det = 1 / det;
	tvec = vector_sub(ray.origin, triangle.a);
	res.u = dot_product(tvec, pvec) * det;
	if (res.u < 0 || res.u > 1)
		return (res);
	qvec = cross_product(tvec, e1);
	res.v = dot_product(ray.direction, qvec) * det;
	if (res.v < 0 || res.u + res.v > 1)
		return (res);	res.z = dot_product(e2, qvec) * det;
	return (res);
}