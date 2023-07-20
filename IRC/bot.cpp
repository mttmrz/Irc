/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarzano <mmarzano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 17:21:02 by bmuni             #+#    #+#             */
/*   Updated: 2023/06/07 12:56:18 by mmarzano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void botName(sbase *base)
{
	srand((unsigned) time(NULL));
	int random = 0;
	random = rand() % 14;
	if (random == 1)
		base->bot = "Kevin";
	else if (random == 2)
		base->bot = "Lenny";
	else if (random == 3)
		base->bot = "S.Pertini";
	else if (random == 4)
		base->bot = "G.Scotty";
	else if (random == 5)
		base->bot = "G.Meloni";
	else if (random == 6)
		base->bot = "Lapo";
	else if (random == 7)
		base->bot = "Chioccini";
	else if (random == 8)
		base->bot = "Ugo";
	else if (random == 9)
		base->bot = "Craig";
	else if (random == 10)
		base->bot = "L";
	else if (random == 11)
		base->bot = "LIGHT";
	else if (random == 12)
		base->bot = "OpenAi";
	else if (random == 13)
		base->bot = "C.Gpt";
	else if (random == 14)
		base->bot = "Bender";	
	else
	base->bot = "T.Padre";
}

void getTime(sbase *base)
{
	string timestamp;
	time_t _tm = time(NULL );
	struct tm * curtime = localtime ( &_tm );
	timestamp = asctime(curtime); 
	base->time = timestamp;
	
}

void	ft_time(int fd, sbase *base)
{
	string timestamp;
	string res;

	res = base->bot + " It's time to get a watch!\r\n";
	time_t _tm = time(NULL );
	struct tm * curtime = localtime ( &_tm );
	send(fd, res.c_str(), res.length(), 0);
	res = base->bot + " Just kidding... the current time is: ";
	send(fd, res.c_str(), res.length(), 0);
	timestamp = asctime(curtime); 
	base->time = timestamp;
	timestamp = timestamp + "\r\n";
	send(fd, timestamp.c_str(), timestamp.length(), 0);
	send(fd, "\r\n", 2, 0);
}

void	ft_help(int fd, sbase *base)
{
	string response;
	map<int, User *>::iterator it = base->usr.ppl.find(fd);
	if (it != base->usr.ppl.end())
	{
		response = base->bot + " Hello " + it->second->getNick() + "!\r\n";
		send(fd, response.c_str(), response.length(), 0);
	}
	response = base->bot + " Here is the available commands list:\n" + base->bot + " !help - displays this message\n" + base->bot + " !pong\n" + base->bot + " !time\n" + base->bot + " !alive?\n" + base->bot + " !slave\n" + base->bot + " !users\n" + base->bot + " !channels\n" + base->bot + " !cockatiel\n"+ base->bot + " !duck\n" + base->bot + " !pigeon\n" + base->bot + " !parrot\n" + base->bot + " Only you can see the messages you send to the bot and vice versa.\r\n";
	send(fd, response.c_str(), response.length(), 0);
}

void	ft_slave(int fd, sbase *base)
{
	string res;
	
	map<int, User *>::iterator it = base->usr.ppl.find(fd);
	if (it != base->usr.ppl.end() && (it->second->getNick() == "bmuni" || it->second->getNick() == "mmarzano" || it->second->getAdmin() == true))
		{
			res = base->bot + " Yes, master!\r\n";
			send(fd, res.c_str(), res.length(), 0);
		}
	else
	{
		res = base->bot + " You are not my master!\r\n";
		send(fd, res.c_str(), res.length(), 0);
	}
}

void	ft_listusr(int fd, sbase *base)
{
	string response;
	map<int, User *>::iterator it = base->usr.ppl.find(fd);
	if (it != base->usr.ppl.end())
	{
		response = base->bot + " Hello " + it->second->getNick() + "!\r\n";
		send(fd, response.c_str(), response.length(), 0);
	}
	string res;
	res = base->bot + " Here is the list of users connected to the server:\r\n";
	send(fd, res.c_str(), res.length(), 0);
	res = base->bot + " ";
	send(fd, res.c_str(), res.length(), 0);
	for (it = base->usr.ppl.begin(); it != base->usr.ppl.end(); it++)
	{
		response = it->second->getNick();
		send(fd, response.c_str(), response.length(), 0);
		if (++it == base->usr.ppl.end())
			break;
		else
		{
			send(fd, ", ", 2, 0);
			it--;
		}
	}
	send(fd, "\r\n", 2, 0);
}

void	ft_listchn(int fd, sbase *base)
{
	string response;
	string res;
	map<int, User *>::iterator it = base->usr.ppl.find(fd);
	if (it != base->usr.ppl.end())
	{
		response = base->bot + " Hello " + it->second->getNick() + "!\r\n";
		send(fd, response.c_str(), response.length(), 0);
	}
	res = base->bot + " Here is the list of channels:\r\n";
	send(fd, res.c_str(), res.length(), 0);
	res = base->bot + " ";
	send(fd, res.c_str(), res.length(), 0);
	for (map<string, Channel *>::iterator ch = base->ch.chlist.begin(); ch != base->ch.chlist.end(); ch++)
	{
		response = ch->second->getName();
		send(fd, response.c_str(), response.length(), 0);
		if (++it == base->usr.ppl.end())
			break;
		else
		{
			send(fd, ", ", 2, 0);
			it--;
		}
	}
	send(fd, "\r\n", 2, 0);
}

void	ft_parrots(int fd)
{
		send(fd, "bot ", 4, 0);
			send(fd, "____________________________________________________________________________________________\n", 93, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "______________██████████_______________________________________________________________\n", 108, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____________██░░░░░░░░░░████________________________________________________________\n", 117, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "__________██░░░░░░░░░░░░░░░░██____________________________________________________\n", 123, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "________██░░░░░░░░░░░░░░░░░░░░██________________________________________________\n", 129, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "______██░░░░░░██░░░░░░░░░░░░████_______________________________________________\n", 132, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____██░░░░░░░░██░░████████░░██░░██___________________________________________\n", 138, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____██░░░░░░░░░░░░██░░░░░░██░░░░██___________________________________________\n", 138, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "__████░░░░░░░░░░░░██░░░░░░██░░░░░░██_______________________________________\n", 144, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "__██░░░░░░░░░░░░░░██░░░░░░██░░░░░░░░██____________________________________\n", 147, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "██░░░░░░░░░░░░░░░░██░░░░████░░░░░░░░██___________________________________\n", 150, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "██░░░░░░░░░░░░░░░░░░██░░██░░░░░░░░░░██___________________________________\n", 150, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "██░░░░░░░░░░░░░░░░░░██░░██░░░░░░░░░░██___________________________________\n", 150, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "__██░░░░░░░░░░░░░░░░░░██░░░░░░░░░░░░██____________________________________\n", 147, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____██░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██__________________________________\n", 147, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____██░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░████____________________________\n", 153, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██_________________________\n", 156, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____██░░██████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██______________________\n", 159, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____██░░░░░░░░██░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██___________________\n", 162, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "____████████████████████████████████████████████████________________\n", 165, 0);
			send(fd, "bot ", 4, 0);
			send(fd, "______________________________________________________________________________________________\n", 95, 0);
}

void	ft_mua(int fd, sbase *base)
{
	string res;

	res = base->bot + " ♡\n\r";
	send(fd, res.c_str(), res.length(), 0);
}

int	ft_safe_words(string tofind, int fd, sbase *base)
{
	int i = -1;
	tofind = ft_toupper(tofind);
	string res;
	string banned[24] = {"BOT", "FROCIO", "NEGRO", "RICCHIONE", "FANCULO", "CAZZO", "COGLIONE", "TROIA", "PUTTANA", "BITCH", "DICK", "DUMB", "CULO", "FOTTI", "FUCK", "ASS", "NIGGA", "NIGER", "COCK", "TIT", "TETTE", "FIGA", "VAGINA", "END"};
	res = base->bot +" that's an offensive message, you can't send it!\r\n";
	while (banned[++i].c_str())
	{
		if (banned[i] == "END")
				break ;
		if (tofind.find(banned[i]) != string::npos)
		{
			send(fd, res.c_str(), res.length(), 0);
			return (-1);
		}
	}
	return (0);
}

int	ft_safe_nicks(string toCheck, int fd, sbase *base)
{
	int i = -1;
	toCheck = ft_toupper(toCheck);
	string res;
	string banned[24] = {"BOT", "FROCIO", "NEGRO", "RICCHIONE", "FANCULO", "CAZZO", "COGLIONE", "TROIA", "PUTTANA", "BITCH", "DICK", "DUMB", "CULO", "FOTTI", "FUCK", "ASS", "NIGGA", "NIGER", "COCK", "TIT", "TETTE", "FIGA", "VAGINA", "END"};

	res = base->bot + " that's an offensive name, change it!\r\n";
	while (banned[++i].c_str())
	{
		if (banned[i] == "END")
				break ;
		if (toCheck.find(banned[i]) != string::npos)
		{
			send(fd, res.c_str(), res.length(), 0);
			return (-1);
		}
	}
	return (0);
}

int	ft_safe_topic(string tofind, int fd, sbase *base)
{
	int i = -1;
	tofind = ft_toupper(tofind);
	string banned[24] = {"BOT", "FROCIO", "NEGRO", "RICCHIONE", "FANCULO", "CAZZO", "COGLIONE", "TROIA", "PUTTANA", "BITCH", "DICK", "DUMB", "CULO", "FOTTI", "FUCK", "ASS", "NIGGA", "NIGER", "COCK", "TIT", "TETTE", "FIGA", "VAGINA", "END"};
	while (banned[++i].c_str())
	{
		if (banned[i] == "END")
				break ;
		if (tofind.find(banned[i]) != string::npos)
		{
			string res = base->bot + " that's an offensive topic, you can't put it!\r\n";
			send(fd, res.c_str(), res.length(), 0);
			return (-1);
		}
	}
	return (0);
}

void	ft_bot(string message, sbase *base, int fd)
{
	string res;
	if (ft_toupper(message) == "!PONG\n")
	{
		res = base->bot + " PING\r\n";
		send(fd, res.c_str(), res.length(), 0);
	}
	else if (ft_toupper(message) == "!ALIVE?\n")
	{
		res = base->bot + " whatzzup?\r\n";
		send(fd, res.c_str(), res.length(), 0);
	}
	else if (ft_toupper(message) == "!TIME\n")
		ft_time(fd, base);
	else if (ft_toupper(message) == "!HELP\n")
		ft_help(fd, base);
	else if (ft_toupper(message) == "!SLAVE\n")
		ft_slave(fd, base);
	else if (ft_toupper(message) == "!USERS\n")
		ft_listusr(fd, base);
	else if (ft_toupper(message) == "!CHANNELS\n")
		ft_listchn(fd, base);
	else if (ft_toupper(message) == "!DUCK\n")
		send(fd, "bot https://youtu.be/0Da8ZhKcNKQ\r\n", 34, 0);
	else if (ft_toupper(message) == "!PIGEON\n")
		send(fd, "bot https://youtu.be/3K2CNbMvjtU\r\n", 34, 0);
	else if (ft_toupper(message) == "!COCKATIEL\n")
		send(fd, "bot https://youtu.be/KTpuOSex218\r\n", 34, 0);
	else if (ft_toupper(message) == "!PARROT\n")
		ft_parrots(fd);
	else if (ft_toupper(message) == "!MUA\n")
		ft_mua(fd, base);
	else
	{
		res = base->bot + " I don't understand you\r\n";
		send(fd, res.c_str(), res.length(), 0);
	}
}