#include "codexion.h"

int	ft_atoi(const char *nptr)
{
	int (numbre), (positive);
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

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2 || ft_strlen(s1) != ft_strlen(s2))
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}