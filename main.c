/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braimbau <braimbau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 10:38:01 by selgrabl          #+#    #+#             */
/*   Updated: 2019/11/30 00:31:11 by braimbau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libc.h>
#include <pthread.h>
#include <mlx.h>

#define NB_CORE (16)

typedef struct		s_send
{
	int				i;
	pthread_mutex_t *mutex;
	char			*id;
	void			*img;
	int				tiny_x;
	int				res_y;
}					t_send;


void				*test(void *arg)
{
	t_send	*send;

	send = (t_send*) arg;
	
	int x = 0;
	while (x < send->tiny_x)
	{
		int y = 0;
		while (y < send->res_y)
		{
			if (send->i % 2 == 0)
				send->id [(x + y * send->tiny_x) * 4] = 50; //B
			else
				send->id [(x + y * send->tiny_x) * 4] = 255; //B
			if (send->i % 3 == 0)
				send->id [(x + y * send->tiny_x) * 4 + 1] = 50; //G
			else if (send->i % 3 == 1)
				send->id [(x + y * send->tiny_x) * 4 + 1] = 150; //G
			else
				send->id [(x + y * send->tiny_x) * 4 + 1] = 255; //G
			if (send->i % 4 == 0)
				send->id [(x + y * send->tiny_x) * 4 + 2] = 50; //R
			else if (send->i % 3 == 1)
				send->id [(x + y * send->tiny_x) * 4 + 2] = 125; //R
			else if (send->i % 3 == 2)
				send->id [(x + y * send->tiny_x) * 4 + 2] = 185; //R
			else
				send->id [(x + y * send->tiny_x) * 4 + 2] = 255; //R
			y++;
		}
		x++;
	}
}

int 				main()
{
	t_send			send[NB_CORE];
	pthread_mutex_t mutex;
	pthread_t		thread[NB_CORE];
	int i;

	int res_x = 1600;
	int res_y = 1000;

	int tiny_x = res_x / NB_CORE;
	if (tiny_x * NB_CORE != res_x)
		tiny_x++;
	printf("tiny_x = %d\n", tiny_x);
	void	*mlx_ptr;
	void	*win_ptr;
	
	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, res_x, res_y, "hey salut");

	i = 0;
	pthread_mutex_init(&(mutex), NULL);
	while (i < NB_CORE)
	{
		int     bpp;
		int     size_line;
		int     endian;
		send[i].img = mlx_new_image(mlx_ptr, tiny_x, res_y);
		send[i].i = i;
		send[i].mutex = &mutex;
		send[i].id = mlx_get_data_addr(send[i].img, &bpp, &size_line, &endian);
		send[i].tiny_x = tiny_x;
		send[i].res_y = res_y;
		pthread_create(&thread[i], NULL, &test, &send[i]);
		i++;
	}
	while (i >= 0)
	{
		pthread_join(thread[i], NULL);
		mlx_put_image_to_window(mlx_ptr, win_ptr, send[i].img, i * tiny_x, 0);
		i--;
	}
	mlx_loop(mlx_ptr);
	return (0);
}
