/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selgrabl <selgrabl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 10:38:01 by selgrabl          #+#    #+#             */
/*   Updated: 2019/11/29 12:58:44 by selgrabl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libc.h>
#include <pthread.h>

typedef struct		s_send
{
	int				i;
	pthread_mutex_t *mutex;
}					t_send;


void				*test(void *arg)
{
	t_send	*send;

	send = (t_send*) arg;
	pthread_mutex_lock (send->mutex);
	printf("This is thread number %d \n", send->i);
	pthread_mutex_unlock (send->mutex);
	return (NULL);
}

int 				main()
{
	t_send			send[5];
	pthread_mutex_t mutex;
	pthread_t		thread[5];
	int i;

	i = 0;
	pthread_mutex_init(&(mutex), NULL);
	while (i < 5)
	{
		send[i].i = i;
		send[i].mutex = &mutex;
		pthread_create(&thread[i], NULL, &test, &send[i]);
		i++;
	}
	while (i)
	{
		pthread_join(thread[i], NULL);
		i--;
	}
	return (0);
}
