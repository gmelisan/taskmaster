/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   taskmaster.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 05:01:40 by gmelisan          #+#    #+#             */
/*   Updated: 2020/09/28 05:01:44 by gmelisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKMASTER_H
#define TASKMASTER_H

#include <string>
#include <atomic>

#define PROGRAMNAME             "taskmasterd"
#define PROGRAMVERSION          "0.1.0"
#define DEFAULT_CONFIG_PATH     "taskmaster-config.yml"
#define LOGPATH                 "taskmasterd.log"
#define QUIT_WAIT_TIMEOUT		3

extern std::string config_path;

#endif