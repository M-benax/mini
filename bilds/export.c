/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:03:48 by aaboudra          #+#    #+#             */
/*   Updated: 2025/07/16 18:23:19 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	update_existing_var(t_env *t, char *val, int p_ec, t_data *data)
{
	char	*joined;

	if (val != NULL)
	{
		if (p_ec && t->val != NULL)
		{
			joined = ft_strjoin(t->val, val, data);
			gc_free_ptr(t->val, data);
			gc_free_ptr(val, data);
			t->val = joined;
		}
		else
		{
			gc_free_ptr(t->val, data);
			t->val = val;
		}
	}
	else
		gc_free_ptr(val, data);
}

static	t_env	*find_var(t_env *envp, char *var)
{
	t_env	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (ft_strcmp(tmp->var, var) == 0)
			return (tmp);
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

static	void	export(char *arg, t_env *envp, t_data *data)
{
	char	*var;
	char	*val;
	int		is_plus_equal;
	t_env	*tmp;

	var = get_var_plus_equal(arg, data);
	if (ft_strcmp(var, "s_pwd") == 0)
		return ;
	val = get_val_plus_equal(arg, data);
	is_plus_equal = kontine_puse(arg);
	tmp = find_var(envp, var);
	if (tmp && ft_strcmp(tmp->var, var) == 0)
		update_existing_var(tmp, val, is_plus_equal, data);
	else
		tmp->next = new_env_node(var, val, data);
}

static	int	validation_key(char *export)
{
	int	i;

	if (!export || export[0] == '\0')
		return (0);
	if (!(export[0] == '_' || (export[0] >= 'a' && export[0] <= 'z')
			|| (export[0] >= 'A' && export[0] <= 'Z')))
		return (0);
	i = 1;
	while (export[i] && export[i] != '='
		&& !(export[i] == '+' && export[i + 1] == '='))
	{
		if (export[i] == ' ')
			return (0);
		if (!(export[i] == '_' || (export[i] >= 'a' && export[i] <= 'z')
				|| (export[i] >= 'A' && export[i] <= 'Z')
				|| (export[i] >= '0' && export[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}

int	handel_export(t_data *data)
{
	int		i;
	int		status;

	status = 0;
	i = 1;
	while (data->com->args[i])
	{
		if (!validation_key(data->com->args[i]))
		{
			printf("`%s': not a valid identifier\n", data->com->args[i]);
			status = EXIT_GENERAL_ERROR;
			i++;
			continue ;
		}
		export(data->com->args[i], data->tenv, data);
		i++;
	}
	return (status);
}
