# Irc
My 42 project for IRC


This is my implementation of the 42school project ft_irc (fyi ft stands for "fortytwo") and it is an implementation of an IRC server

I coded this with Bianca Muni, a student from 42Florence .

It is desingned to work with 'Konversation' client, therefore don't expect it to work as good with other Irc Clients.

You can test it with nc (netcat , from terminal) aswell, in order to do so you need to start the server , open another teminal type the address (e.g 10.12.4.4) and the port that you choose. 
Then type (YES caplocks it is important) PASS <yourpw> /n NICK <yournick> /n USER <youruser>.
Rememeber that Irc follows a protocol, so if for example you want to write a message to another client you need to use PRIVMSG : , to join a channel you need to JOIN #<channel> etc. 
Read some documentation if you want to know more (or look inside my code for the right format that you need to use)

Enjoy
