#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_com
{
    char    type;
    float   x, y;
    float   radius;
    char    ch;
} t_com;

typedef struct s_zone
{
    int     width, height;
    char    background;
} t_zone;

int ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);
}

void    ft_putstr(char *str)
{
    write(1, str, ft_strlen(str));
}

void    draw(char   **arr)
{
    while (*arr)
    {
        ft_putstr(*arr);
        write(1, "\n", 1);
        arr++;
    }
}

int get_zone(FILE *stream, t_zone *zone)
{
    int elements;

    elements = fscanf(stream, "%d %d %c\n", &zone->width, &zone->height, &zone->background);
    if (elements <= 0)
        return (0);
    return (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300) ? elements : 1;
}

int    get_command(FILE *stream, t_com *com)
{
    int elements;

    elements = fscanf(stream, "%c %f %f %f %c\n",
       &com->type, &com->x, &com->y, &com->radius, &com->ch);
    if (elements <= 0)
        return (0);
    return (com->radius > 0.0f && (com->type == 'c' || com->type == 'C')) ? elements : 1;
}

char    **zone_create(t_zone *zone)
{
    int i = 0;
    char **arr = malloc(sizeof(char*)*(zone->height + 1));
    while (i < zone->height)
    {
      arr[i] = malloc(zone->width + 1);
      i++;
    }
    i = 0;
    int j = 0;
    while (i < zone->height)
    {
        j = 0;
        while (j < zone->width)
        {
            arr[i][j] = zone->background;
            j++;
        }
        arr[i][j] = '\0';
        i++;
    }
    arr[i] = NULL;
    return (arr);
    
}

void    close_free(char **arr, FILE *stream)
{
    char    **tmp1 = arr;
    char    *tmp2;
    if (stream)
        fclose(stream);
    if (!tmp1)
        return ;
    while (*tmp1)
    {   
        tmp2 = *tmp1;
        free(tmp2);
        tmp1++;
    }
    free(arr);
}

void    create_circle_filled(char **arr, t_zone *zone, t_com *com)
{
    int i = 0;
    int j;
    while (i < zone->height)
    {
        j = 0;
        while (j < zone->width)
        {
            if (sqrtf((com->x - (float) j)*(com->x - (float) j) + (com->y - (float) i)*(com->y - (float) i)) <= com->radius)
                arr[i][j] = com->ch;
            j++;
        }
        i++;
    }
}

void    create_circle_empty(char **arr, t_zone *zone, t_com *com)
{
    int i = 0;
    int j;
    float distance;
    while (i < zone->height)
    {
        j = 0;
        while (j < zone->width)
        {
            distance = sqrtf((com->x - (float) j)*(com->x - (float) j) + (com->y - (float) i)*(com->y - (float) i));
            if (distance <= com->radius)
                if (distance - com->radius < 1.0f && distance - com->radius > -1.0f)
                arr[i][j] = com->ch;
            j++;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    t_com com;
    t_zone zone;
    FILE *stream;
    char    **arr;
    int     number;

    arr = 0;
    if (argc != 2)
    {
        ft_putstr("Error:argument\n");
        return (1);
    }
    stream = fopen(*(argv+1), "r");
    
    if (get_zone(stream, &zone) != 3)
    {
        ft_putstr("Error: Operation file corrupted\n");
        close_free(arr, stream);
        return (1);
    } 
    arr = zone_create(&zone);
    while ((number = get_command(stream, &com)) > 0)
    {
        if (number != 5)
        {
            ft_putstr("Error: Operation file corrupted\n");
            close_free(arr, stream);
            return (1);
        }
        if (com.type == 'C')
            create_circle_filled(arr, &zone, &com);
        else
            create_circle_empty(arr, &zone, &com);
    }
    draw(arr);
    close_free(arr, stream);
    return (0);
}