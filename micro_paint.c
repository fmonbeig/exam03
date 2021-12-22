#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct s_rect
{
	float	width;
	float	height;
	float	xtl;
	float	ytl;
	float	xbr;
	float	ybr;
	char	flag;
	char	c;
}		t_rect;

typedef struct s_zone
{
	int width;
	int height;
	char c;
	char **draw;
}		t_zone;

void ft_putstr(char *s)
{
	while(*s)
		write(1, s++, 1);
}
int	check_zone(t_zone *zone)
{
	if (zone->width > 300 || zone->width <= 0 || zone->height <= 0 || zone->height > 300)
		return(1);
	return(0);
}

int	get_zone(FILE *fd, t_zone *zone)
{
	int i;

	if ((i = fscanf(fd, "%d %d %c\n", &zone->width, &zone->height, &zone->c)) != 3)
		return (1);
	if(check_zone(zone))
		return(1);
	zone->draw = malloc(sizeof(char *) * zone->height + 1);
	i = -1;
	while (++i < zone->height)
	{
		zone->draw[i] = malloc(sizeof(char) * zone->width + 1);
		memset(zone->draw[i], zone->c, zone->width);
		zone->draw[i][zone->width] = '\0';
	}
	zone->draw[i] = NULL;
	return(0);
}

void	draw_all(t_zone *zone)
{
	int i;

	i = -1;

	while (++i < zone->height)
	{
		ft_putstr(zone->draw[i]);
		ft_putstr("\n");
	}
}

int	check_rect(t_rect *rect)
{

	if (rect->flag != 'r' && rect->flag != 'R')
		return(1);
	if(rect->width < 0.00000000 || rect->height < 0.00000000)
		return(1);
	return(0);
}

void calcul_point(t_rect *rect)
{
	rect->xbr = rect->xtl + rect->width;
	rect->ybr = rect->ytl + rect->height;
}

int	is_in_rect(int x, int y, t_rect *rect)
{

	if (x >= rect->xtl && x <= rect->xbr && y >= rect->ytl && y <= rect->ybr)
		return(1);
	return(0);
}

void draw_rectangle(t_rect *rect, t_zone *zone)
{
	int y;
	int x;

	y = -1;
	while(++y < zone->height)
	{
		x = -1;
		while (++x < zone->width)
		{
			if (is_in_rect(x, y, rect))
			{
				if (rect->xbr - x < 1.00000000 || x - rect->xtl < 1.00000000
				|| rect->ybr - y < 1.00000000 || y- rect->ytl <1.00000000)
					zone->draw[y][x] = rect->c;
				else
				{
					if (rect->flag =='R')
						zone->draw[y][x] = rect->c;
				}
			}
		}
	}
}

int	get_rectangle(FILE *fd, t_zone *zone)
{
	int i;
	t_rect rect;
	while((i = fscanf(fd, "%c %f %f %f %f %c\n", &rect.flag, &rect.xtl, &rect.ytl, &rect.width, &rect.height, &rect.c)) == 6)
	{
		if(check_rect(&rect))
			return(1);
		calcul_point(&rect);
		draw_rectangle(&rect, zone);
	}
	if (i == -1)
		return(0);
	else
		return (1);
}


int	main(int argc, char **argv)
{
	t_zone zone;
	FILE *fd;

	if (argc ==2)
	{
		fd = fopen(argv[1], "r");
		if (!fd)
		{
			ft_putstr("Error: Operation file corrupted\n");
			return(1);
		}
		if(get_zone(fd, &zone))
		{
			ft_putstr("Error: Operation file corrupted\n");
			return(1);
		}
		if(get_rectangle(fd, &zone))
		{
			ft_putstr("Error: Operation file corrupted\n");
			return(1);
		}
		 draw_all(&zone);
		return(0);
	}
	else
	{
		ft_putstr("Error: argument\n");
		return(1);
	}
}
