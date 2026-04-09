#include "codexion.h"

long	ft_atoi(const char *nptr)
{
	long (numbre), (positive);
	numbre = 0;
	positive = 1;
	while ((*nptr >= '\t' && *nptr <= '\r') || *nptr == ' ')
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			positive = -positive;
		nptr++;
	}
	while (*nptr)
	{
        if (!(*nptr >= '0' && *nptr <= '9'))
            return -1;
		numbre = (numbre * 10) + (*nptr - 48);
		if (numbre > INT_MAX)
			return (-1);
		nptr++;
	}
	return (numbre * positive);
}

size_t	ft_strlen(const char *s)
{
	const char	*c;

	c = s;
	if (!s)
		return (0);
	while (*s)
		s++;
	return (s - c);
}

char to_lower(char a)
{
	if (a >= 65 && a <= 90)
		return a + 32;
	return a;
}

int	ft_strcmp_adv(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2 || ft_strlen(s1) != ft_strlen(s2))
		return (0);
	while (s1[i] && s2[i])
	{
		if (to_lower(s1[i]) != to_lower(s2[i]))
			return (0);
		i++;
	}
	return (1);
}

void smart_sleep(long time, t_table *t)
{
    long start = gettimenow();
	long time_to_sleep = time / 10;
    while (!get_stop(t))
    {
        if (gettimenow() - start >= time)
            break;
        usleep(time_to_sleep);
    }
}