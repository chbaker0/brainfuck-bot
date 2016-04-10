#include "simulator.hpp"

#include <vector>
#include <time.h>

using namespace std;

bool simulate(const std::string& code, const std::string& input, std::string& output, unsigned int timeout_ms, unsigned int mem_size)
{
	vector<unsigned char> mem(mem_size, 0);
	vector<unsigned int> jump_stack;

	unsigned int code_index = 0, mem_index = 0, input_index = 0;

	struct timespec timeout{0, 0};
	clock_gettime(CLOCK_MONOTONIC, &timeout);
	timeout.tv_nsec += (timeout_ms * 1000) % 1000000;
	timeout.tv_sec += (timeout_ms * 1000) / 1000000;

	while(code_index < code.size())
	{
		struct timespec t{0, 0};
		clock_gettime(CLOCK_MONOTONIC, &t);
		if(t.tv_sec >= timeout.tv_sec && t.tv_nsec >= timeout.tv_nsec)
			return false;

		char c = code[code_index];
		if(c == '+')
		{
			mem[mem_index]++;
		}
		else if(c == '-')
		{
			mem[mem_index]--;
		}
		else if(c == '<')
		{
			++mem_index;
			if(mem_index == mem_size)
				mem_index = 0;
		}
		else if(c == '>')
		{
			if(mem_index == 0)
				mem_index = mem_size;
			--mem_index;
		}
		else if(c == '.')
		{
			output.push_back(mem[mem_index]);
		}
		else if(c == ',')
		{
			if(input_index < input.size())
				mem[mem_index] = input[input_index++];
			else
				mem[mem_index] = 0;
		}
		else if(c == '[')
		{
		    if(mem[mem_index] == 0)
			{
				unsigned int loop_count = 1;
				for(unsigned int i = code_index + 1; i < code.size(); ++i)
				{
					if(code[i] == '[')
						++loop_count;
					else if(code[i] == ']')
						--loop_count;

					if(loop_count == 0)
					{
						code_index = i;
						break;
					}
				}

				if(loop_count != 0)
					return false;
			}
			else
			{
				jump_stack.push_back(code_index);
			}
		}
		else if(c == ']')
		{
			if(mem[mem_index] != 0)
			{
				if(jump_stack.size() == 0)
					return false;
				
				code_index = jump_stack.back();
			}
			else
			{
				if(jump_stack.size() == 0)
					return false;
				
				jump_stack.pop_back();
			}
		}

		++code_index;
	}

	return true;
}
