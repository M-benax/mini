/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:54 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 19:36:57 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int builtin_pwd(t_env *env_list, t_data *data) // <<<<<< تم تعديل التوقيع
{
    char *pwd_val;
    char current_path_buffer[PATH_MAX];

    // 1. حاول الحصول على PWD من البيئة
    pwd_val = get_env_value(env_list, "PWD", data); // افترض أن get_env_value ترجع "" إذا لم يُعثر عليه أو كان فارغًا

    if (pwd_val && pwd_val[0] != '\0') // إذا كان PWD موجودًا وله قيمة
    {
        write(STDOUT_FILENO, pwd_val, ft_strlen(pwd_val));
        write(STDOUT_FILENO, "\n", 1);
        gc_free_ptr(pwd_val, data); // إذا كان get_env_value يخصص ذاكرة (يفترض أنه يفعل gc_malloc)
        return (0);
    }
    gc_free_ptr(pwd_val, data); // حرر السلسلة الفارغة إذا تم تخصيصها

    // 2. إذا لم يكن PWD موجودًا أو كان فارغًا، استخدم getcwd() كـ fallback
    if (getcwd(current_path_buffer, PATH_MAX) == NULL)
    {
        // هنا يمكن أن يحدث خطأ "No such file or directory" إذا كان المسار الحالي الفعلي محذوفًا
        display_error_message("pwd", NULL, strerror(errno));
        return (1);
    }
    write(STDOUT_FILENO, current_path_buffer, ft_strlen(current_path_buffer));
    write(STDOUT_FILENO, "\n", 1);
    return (0);
}