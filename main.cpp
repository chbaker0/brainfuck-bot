#include <iostream>
#include <functional>
#include <string>

#include <tgbot/tgbot.h>

#include "simulator.hpp"

void handle_start(TgBot::Bot& bot, TgBot::Message::Ptr msg)
{
	bot.getApi().sendMessage(msg->chat->id, "Fuck off");
}

void handle_run(TgBot::Bot& bot, TgBot::Message::Ptr msg)
{
	std::string code, input, output;

	std::string& str = msg->text;
	
	unsigned int i = 0;
	while(i < str.size() && str[i++] != ' ');

	while(i < str.size() && str[i] != ' ')
		code.push_back(str[i++]);

	++i;
	while(i < str.size())
		input.push_back(str[i++]);

	if(simulate(code, input, output, 1000, 2048))
	{
		if(output.size() == 0)
			output = "No output";
		else
			output = "Output:\n\n" + output;
		
		std::string output_utf8;
		unsigned int i = 0;
		for(char c : output)
		{
			if(i++ >= 256) break;
			
			unsigned char uc = c;
			if(uc < 128)
			{
				output_utf8.push_back(c);
			}
			else
			{
				output_utf8.push_back(0b11000000 | (uc >> 6));
				output_utf8.push_back(0b10000000 | (uc & 0b00111111));
			}
		}
		bot.getApi().sendMessage(msg->chat->id, output_utf8);
	}
	else
	{
		bot.getApi().sendMessage(msg->chat->id, "You got cucked by the timeout");
	}
}

int main()
{
	using namespace TgBot;
	using namespace std;

	Bot bot("187960084:AAFFx1A-QCRgn_BvyvR8tSAEQW92haAl4-E");
	bot.getEvents().onCommand("start", std::bind(handle_start, std::ref(bot), std::placeholders::_1));
	bot.getEvents().onCommand("run", std::bind(handle_run, std::ref(bot), std::placeholders::_1));
	try
	{
		cout << bot.getApi().getMe()->username << endl;
		TgBot::TgLongPoll long_poll(bot);
		while(true)
		{
			long_poll.start();
		}
	}
	catch(TgBot::TgException& e)
	{
		cerr << e.what() << endl;
		return 1;
	}

	return 0;
}
