#include "irc.hpp"


User::User(void)
{
    return ;
}

Users::Users(void)
{
    return ;
}

Users::~Users(void)
{
    map<int, User *>::iterator it = ppl.begin();
    while (it != ppl.end())
    {
        delete it->second;
        it++;
    }
    return ;
}

std::string &User::getUsr(void)
{
    return this->_usr;
}

std::string &User::getNick(void)
{
    return this->_nick;
}

std::string &User::getPass(void)
{
    return this->_pass;
}

bool &User::getAdmin(void)
{
    return this->_admin;
}

bool &User::getAuth(void)
{
    return (this->_auth);
}

void User::setAuth(bool b)
{
    this->_auth = b;
    return ;
}

void User::setNick(const std::string &n)
{
    this->_nick = n;
    return ;

}

void User::setUsr(const std::string &n)
{
    this->_usr = n;
    return ;

}

void User::setPass(const std::string &n)
{
    this->_pass = n;
    return ;

}

void User::setAdmin(bool b)
{
    this->_admin = b;
    return ;
}

User::~User(void)
{
    return ;
}

Channel::Channel(void)
{
    return ;
}

Channel::~Channel(void)
{   
    return ;
}

string &Channel::getName(void)
{
    return (this->name);
}

bool &Channel::getPriv(void)
{
    return (this->priv);
}

bool &Channel::getInvite(void)
{
    return (this->invite);
}

bool &Channel::getRtopic(void)
{
    return (this->rtopic);
}

bool &Channel::getKey(void)
{
    return (this->key);
}

bool Channel::getMax(void)
{
    return (this->max);
}

string &Channel::getTopic(void)
{
    return (this->topic);
}

int 	Channel::getNMax(void)
{
    return (this->nmax);
}

vector<string> &Channel::getUsrs(void)
{
    return (this->usrs);
}

deque<string> &Channel::getAdmins(void)
{
    return (this->admins);
}

deque<string> &Channel::getInvited(void)
{
    return (this->invited);
}

string		Channel::getPass(void)
{
    return (this->pass);
}

void	Channel::setPass(const string &n)
{
    this->pass = n;
    return ;
}

void    Channel::setName(const string &n)
{
    this->name = n;
    return ;
}

void    Channel::setPriv(bool b)
{
    this->priv = b;
    return ;
}

void    Channel::setInvite(bool b)
{
    this->invite = b;
    return ;
}

void    Channel::setRtopic(bool b)
{
    this->rtopic = b;
    return ;
}

void    Channel::setKey(bool b)
{
    this->key = b;
    return ;
}

void    Channel::setMax(bool b)
{
    this->max = b;
    return ;
}

void    Channel::setTopic(const string &n)
{
    this->topic = n;
    return ;
}

void    Channel::setNMax(int n)
{
    this->nmax = n;
    return ;
}

void    Channel::addUsr(const string &n)
{
    this->usrs.push_back(n);
    return ;
}

void    Channel::addAdmin(const string &n)
{
    this->admins.push_back(n);
    return ;
}

void    Channel::addInvited(const string &n)
{
    this->invited.push_back(n);
    return ;
}

void    Channel::delInv(const string &n)
{
     deque<string>::iterator it = this->invited.begin();
    for (; it != this->invited.end(); it++)
    {
        if (*it == n)
            break ;
    }
    this->invited.erase(it);
    return ;
}

void    Channel::delUsr(const string &n)
{
    vector<string>::iterator it = this->usrs.begin();
    for (; it != this->usrs.end(); it++)
    {
        if (*it == n)
        {
            this->usrs.erase(it);
            break ;
        }
    }
    if (it == this->usrs.end())
    {
        this->usrs.clear();
        return ;
    }
    return ;
}

void    Channel::delAdmin(const string &n)
{
    deque<string>::iterator it = this->admins.begin();
    for (; it != this->admins.end(); it++)
    {
        if (*it == n)
            break ;
    }
    this->admins.erase(it);
    return ;
}