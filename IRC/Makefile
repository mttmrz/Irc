# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmuni <bmuni@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/08 17:54:12 by bmuni             #+#    #+#              #
#    Updated: 2023/06/06 15:45:34 by bmuni            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN='\033[0;34m'

NAME=ircserv

all: $(NAME)

$(NAME): 
	c++ -g -Wall -Wextra -Werror -std=c++98 irc.cpp init.cpp bot.cpp main.cpp checks.cpp finder.cpp modes.cpp destroyer.cpp channel.cpp commands.cpp utils.cpp -o $(NAME)
	@clear
	@echo ${GREEN} "ircserv compiled"

clean:
	rm -f $(NAME)
	@clear
	@echo ${GREEN} "all cleaned"

fclean:	clean
	@clear
	@echo ${GREEN} "fcleaned"

re: fclean $(NAME)

.PHONY: all, $(NAME), clean, fclean, re
