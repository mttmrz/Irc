/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarzano <mmarzano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:57:42 by bmuni             #+#    #+#             */
/*   Updated: 2023/06/07 12:57:04 by mmarzano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <signal.h>
#include <deque>
#include <fcntl.h>
#include <algorithm>


extern int check;
using namespace std;

#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

class User
{
	private:
		std::string _nick;
		std::string _pass;
		std::string _usr;
		bool	_auth;
		bool	_admin;
	public:
		User(void);
		~User();
		std::string &getUsr(void);
		std::string &getPass(void);
		std::string &getNick(void);
		bool &getAuth(void);
		void setAuth(bool b);
		void setNick(const std::string &n);
		void setPass(const std::string &n);
		void setUsr(const std::string &n);
		bool	&getAdmin(void);
		void setAdmin(bool b);
};

class Users
{
	public:
	Users(void);
	~Users(void);

	std::map<int, User *> ppl;
};

class Channel
{
	private:
		string name;
		string topic;
		string	pass;
		bool	priv;
		bool	invite;
		bool	rtopic;
		bool	key;
		bool	max;
		int	nmax;
	public:
		Channel(void);
		~Channel(void);
		vector<string> usrs;
		vector<int>usrs_fds;
		deque<string> invited;
		deque<string> admins;
		string &getName(void);
		bool &getPriv(void);
		bool &getInvite(void);
		bool &getRtopic(void);
		bool &getKey(void);
		bool getMax(void);
		string &getTopic(void);
		int 	getNMax(void);
		string	getPass(void);
		vector<string> &getUsrs(void);
		deque<string> &getAdmins(void);
		deque<string> &getInvited(void);
		void	setPass(const string &n);
		void setName(const string &n);
		void setPriv(bool b);
		void setInvite(bool b);
		void setRtopic(bool b);
		void setKey(bool b);
		void setMax(bool b);
		void setTopic(const string &n);
		void setNMax(int n);
		void addUsr(const string &n);
		void addAdmin(const string &n);
		void addInvited(const string &n);
		void delInv(const string &n);
		void delUsr(const string &n);
		void delAdmin(const string &n);
};

class Channels
{
	public:
	Channels(void){};
	~Channels(void)
	{
		map<string, Channel *>::iterator it = chlist.begin();
		while (it != chlist.end())
		{
			delete it->second;
			it++;
		}
	};

	map<string, Channel *> chlist;
};

typedef struct base
{
	int	sockfd;
	uint16_t port;
	std::string password;
	string	saves[10000];
	Users usr;
	string ip;
	int bytes_received;
	Channels ch;
	vector<int> connected;
	vector<int> registered;
	vector<int> nicked;
	string nick;
	string ussr;
	string rules;
	string bot;
	string time;
} sbase;

void getTime(sbase *base);
void ft_accept(sbase* base);
void ft_sighand(int sig);
void	ft_bot(string message, sbase *base, int fd);
int	ft_safe_words(string tofind, int fd, sbase *base);
int	ft_safe_nicks(string toCheck, int fd, sbase *base);
int	ft_safe_topic(string tofind, int fd, sbase *base);
string	ft_toupper(string str);
void	ft_start(sbase *base, char **env);
void	ft_error(std::string type);
void	ft_freeall(sbase *base);
int ft_user(int fd, sbase *base);
void searchAndDestroy(sbase *base, int fd);
int ft_recive(sbase *base, int fd, char *buffer);
int	kickCheck(sbase *base, int fd, string channel);
int checkBuffer(char *buffer, int fd, sbase *base);
int	ft_check_ar(char **av);
int	ft_checkNick(std::string toCheck, int fd, sbase *base);
int		ft_check_chmember(sbase *base, string channel, string name, int fd);
int ft_checkch(sbase *base, string tofind);
int	ft_checkchUsr(sbase *base, int fd, string cn);
int	ft_mode(string message, sbase *base, int fd);
int	ft_mods_righuse(string message, int fd);
void	send_mode_toall(string message, sbase *base, string channel);
bool	is_channel_op(sbase *base, string channel, int fd);
void searchAndDestroy(sbase *base, int fd);
void deLL(sbase *base, int fd, string channel);
void ft_checkOut(string channel, sbase *base, int fd);
void ft_kicker(sbase *base, string message, int fd);
string find_usrs(sbase *base, string channel);
int	multiple_rec(string tofind);
void	find_rec(string tofind, sbase *base, int fd);
int isIn(sbase *base, int fd);
int isReg(sbase *base, int fd);
int isNick(sbase *base, int fd);
void	ft_freeall(sbase *base);
int	multiple_rec(string tofind);
bool	is_channel_op(sbase *base, string channel, int fd);
void	change_admin_usr(sbase *base, string toCheck, string oldNick);
void ft_sighand(int sig);
void	ft_error(std::string type);
string	ft_toupper(string str);
int ft_messageCh(string full, sbase *base, int fd);
int ft_messageToChannelCk(string message, sbase *base);
void sentToCh(sbase *base, string res, string channel);
int	ft_channels(sbase *base, string full, int fd);
int		ft_chModes(sbase *base, string channel, int fd);
void	ft_refreshCH(sbase *base, string full, int fd);
int ft_alreadyINch(sbase *base, int fd, string cn, string res);
void	ft_create_ch(sbase *base, string name, int fd);
int	ft_dcc(string message, sbase *base, int fd);
void	ft_invite(string message, sbase *base, int fd);
void	ft_change_topic(string comm, sbase *base, int fd);
void botName(sbase *base);

#endif