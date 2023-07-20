#include "irc.hpp"

int	check;

void ft_accept(sbase* base)
{
	int clientSocket;
	int clients = 0;
	int	pClients = 0;
	char buffer[1024];
	int status = 0;
	std::vector<pollfd> poll_fds;
	sockaddr_in client_addr = {};
	socklen_t client_addr_len = sizeof(client_addr);
	pollfd data;
    data.fd = base->sockfd;
    data.events = POLLIN;
    data.revents = 0;
	poll_fds.push_back(data);
	base->bytes_received = fcntl(base->sockfd, F_GETFL, 0);
	fcntl(base->sockfd,  F_SETFL, base->bytes_received | O_NONBLOCK);
	while (true) 
	{
		signal(SIGINT, ft_sighand);
		signal(SIGQUIT, ft_sighand);
		signal(SIGTSTP, ft_sighand);
		if (poll(poll_fds.data(), poll_fds.size(), -1) == -1)
			return(ft_freeall(base));
		clientSocket = accept(base->sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
		if (clientSocket > 0)
		{
			std::cout << "\tNew connection accepted\n" << std::endl;
			data.fd = clientSocket;
    		data.events = POLLIN;
    		data.revents = 0;
			poll_fds.push_back(data);
			clients++;
		}
		for (; clients > pClients; pClients++)
		{
			clientSocket = poll_fds[clients].fd;
			bzero(buffer, 1024);
			status = recv(poll_fds[clients].fd, buffer, sizeof(buffer), 0);
			if (ft_recive(base, poll_fds[clients].fd, buffer) != 0 || check == 2)
			{
						if (check == 1)
						return(ft_freeall(base));
				pClients--;
				continue;
			}
		}
		for (size_t i = 1; i < poll_fds.size(); i++)
		{
			if (poll_fds[i].revents & POLLIN)
			{
				bzero(buffer, 1024);
				if (ft_user(poll_fds[i].fd, base) == -1)
				{
					close(poll_fds[i].fd);
					searchAndDestroy(base, poll_fds[i].fd);
				}
			}
		}
		if (check == 1)
			return(ft_freeall(base));
		pClients = clients;
	}
}

string	ft_get_ip(char **env)
{
	string envs;
	for (int i = 0; env[i] != NULL; i++)
	{
		envs = env[i];
		if (envs.find("SESSION_MANAGER") != string::npos)
			break ;
	}
	envs.erase(0, envs.find("/") + 1);
	envs.erase(envs.find(":"), envs.length());
	char *IP;
	struct hostent *host_entry;
	host_entry = gethostbyname(envs.c_str());
	if (host_entry == NULL){
		perror("gethostbyname");
		exit(1);
	}
	IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
	return (IP);
}

int	ft_bind(sbase *base, char **env)
{
    base->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (base->sockfd < 0)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return (-1);
    }
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(base->port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(base->sockfd, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to bind socket." << std::endl;
        close(base->sockfd);
        return (-1);
    }
	std::cout << "Socket created and bound successfully." << std::endl;
    base->ip = ft_get_ip(env);
	return (0);
}

void	ft_start(sbase *base, char **env)
{

	ft_bind(base, env); 

	if (base->sockfd == -1)
		ft_error("ERROR");	
	if (listen(base->sockfd, SOMAXCONN) != 0)
	{
		close(base->sockfd);
		ft_freeall(base);
		ft_error("not listening");
	}
	std::cout << KGRN"Server in ascolto all'indirizzo: " RST << base->ip << KYEL " sulla porta: " RST<< base->port << std::endl;
	base->rules = "IRCSERV You can use the following commands in the main room:\r\nIRCSERV /join <channel_name>\r\nIRCSERV /msg <user> || /query<user>\r\nIRCSERV !help\r\nIRCSERV /dcc send <user>\r\nIRCSERV /quit.\r\n";
	ft_accept(base);
}