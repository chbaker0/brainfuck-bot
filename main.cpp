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
		bot.getApi().sendMessage(msg->chat->id, output);
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
