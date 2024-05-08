/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 14:43:51 by istasheu          #+#    #+#             */
/*   Updated: 2024/05/06 14:43:58 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# ifndef BITS_IN_BYTE
#  define BITS_IN_BYTE 8
# endif

# define COLOR_BLUE "\033[94m"
# define COLOR_CYAN "\033[96m"
# define COLOR_GREEN "\033[32m"
# define COLOR_RED "\033[31m"
# define COLOR_GRAY "\033[90m"
# define COLOR_RESET "\033[0m"

# include <signal.h>
# include "libft_/libft.h"

void    handle_error(const char *context_message);

#endif
