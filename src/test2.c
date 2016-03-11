/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/10 15:13:11 by ybarbier          #+#    #+#             */
/*   Updated: 2016/03/10 15:19:38 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(void)
{

int sock;
char buffer[1024];

struct hostent *hostinfo = NULL;
struct sockaddr_in to = { 0 };
const char *hostname = "www.google.com";
int tosize = sizeof to;

hostinfo = gethostbyname(hostname);
if (hostinfo == NULL)
{
	    fprintf (stderr, "Unknown host %s.\n", hostname);
		exit(1);
}

to.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
to.sin_port = htons(PORT);
to.sin_family = AF_INET;

/*

if(sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&to, tosize) < 0)
{
	    perror("sendto()");
}

if((n = recvfrom(sock, buffer, sizeof buffer - 1, 0, (struct sockaddr *)&to, &tosize)) < 0)
{
	    perror("recvfrom()");
}

buffer[n] = '\0';
*/
return (0);
}
