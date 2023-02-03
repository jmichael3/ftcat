#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#define BUFFSIZE 150

_Bool meets_complexity_requirements(char* line);
void format_line(FILE* fp, unsigned int min_len, unsigned int max_len, char start_char, char end_char, _Bool check_is_complex, _Bool check_is_not_complex);

char* USAGE = "Usage: ftcat [OPTION]... [FILE]...\n\
format FILE(s) and print to standard output.\n\
\n\
With no FILE, or when FILE is -, read standard input.\n\
\n\
  -1\t\tset default options for WPA\n\
  -2\t\tset default options for \"Password must meet complexity requirements\"\n\
  -3\t\tset default options for \"Password does not meet complexity requirements\"\n\
 \n\
  -m\t\tmin word length\n\
  -M\t\tmax word length\n\
  -s\t\tstart character (ascii range)\n\
  -e\t\tend character (ascii range)\n\
  -h\t\tdisplay this help and exit\n\
\n\
Examples:\n\
  ftcat foo bar  Formats foo's contents, then bar's contents.\n\
  ftcat          Copy standard input to standard output.\n";

void format_line(FILE* fp, unsigned int min_len, unsigned int max_len, char start_char, char end_char, _Bool check_is_complex, _Bool check_is_not_complex)
{
	_Bool is_complex = false;
	char chr = '\0';
	unsigned int chr_count = 0;
	char line[BUFFSIZE] = {0};
	while ((chr = getc(fp)) != EOF)
	{
		if (chr != '\n')
		{
			if (chr >= start_char && chr <= end_char && chr_count < BUFFSIZE)
			{
				line[chr_count] = chr;
				chr_count += 1;
			}
			else 
			{
				chr_count = 0;
				while((chr = getc(fp)) != '\n') { continue; }
				
			}
			continue;
		}

		if (chr_count >= min_len && chr_count <= max_len)
		{
			line[chr_count] = '\0';
			is_complex = meets_complexity_requirements(line);
			if (check_is_complex && is_complex)
			{
				printf("%s\n", line);
			}
			else if (check_is_not_complex && !is_complex)
			{
				printf("%s\n", line);
			}
			else if (!check_is_complex && !check_is_not_complex)
			{
				printf("%s\n", line);
			}
		}
		chr_count = 0;

	}



}

_Bool meets_complexity_requirements(char* line)
{
	_Bool meets_complexity = false;
	char complexity[] = {0, 0, 0, 0};
	unsigned int complexity_score = 0;
	char ascii_uppercase_start = 65;
	char ascii_uppercase_end = 90;
	char ascii_lowercase_start = 97;
	char ascii_lowercase_end = 122;
	char ascii_dec_start = 48;
	char ascii_dec_end = 57;
	char spc_chars_1_start = 32;
	char spc_chars_1_end = 47;
	char spc_chars_2_start = 58;
	char spc_chars_2_end = 64;
	char spc_chars_3_start = 91;
	char spc_chars_3_end = 96;
	char spc_chars_4_start = 123;
	char spc_chars_4_end = 126;

	for(int i = 0; i < BUFFSIZE; i++)
	{
		if (line[i] == '\0')
		{
			break;
		}
		else if (line[i] >= ascii_uppercase_start && line[i] <= ascii_uppercase_end && complexity[0] == 0)
		{
			complexity[0] = 1;
		}
		else if (line[i] >= ascii_lowercase_start && line[i] <= ascii_lowercase_end && complexity[1] == 0)
		{
			complexity[1] = 1;
		}
		else if (line[i] >= ascii_dec_start && line[i] <= ascii_dec_end && complexity[2] == 0)
		{
			complexity[2] = 1;
		}
		else if (line[i] >= spc_chars_1_start && line[i] <= spc_chars_1_end && line[i] >= spc_chars_2_start && line[i] <= spc_chars_2_end &&line[i] >= spc_chars_3_start && line[i] <= spc_chars_3_end && line[i] >= spc_chars_4_start && line[i] <= spc_chars_4_end && complexity[3] == 0)
		{
			complexity[3] = 1;
		}

	}

	for (long unsigned int i = 0; i < (sizeof(complexity) * sizeof(char)); i++)
	{
		if (complexity[i] == 1)
		{
			complexity_score += 1;
		}
	}
	if (complexity_score >= 3)
	{
		meets_complexity = true;
	}

	return meets_complexity;

}

int main(int argc, char** argv)
{
	// handle options
	int option;
	unsigned int min_len = 0;
	unsigned int max_len = BUFFSIZE - 1;
	char start_char = 32;
	char end_char = 126;
	_Bool check_is_complex = false;
	_Bool check_is_not_complex = false;
	while((option = getopt(argc, argv, "m:M:s:e:h123")) != -1)
	{
		switch(option)
		{
			case '1':
				min_len = 8;
				max_len = 64;
				start_char = 32;
				end_char = 126;
				check_is_complex = false;
				break;
			case '2':
				min_len = 8;
				max_len = 64;
				start_char = 32;
				end_char = 126;
				check_is_complex = true;
				break;
			case '3':
				min_len = 0;
				max_len = 8;
				start_char = 32;
				end_char = 126;
				check_is_not_complex = true;
				break;
			case 'm':
				min_len = atoi(optarg);
				break;
			case 'M':
				max_len = atoi(optarg);
				break;
			case 's':
				start_char = atoi(optarg);
				break;
			case 'e':
				end_char = atoi(optarg);
				break;
			default:
				printf("%s\n", USAGE);
				return 1;
		}
	}

	// handle remaining args as files
	for (int current_file_index = optind; current_file_index < argc; current_file_index++)
	{
		FILE* fp;
		if ('-' == *argv[current_file_index])
		{
			fp = stdin;
		}

		if (fp != stdin)
		{
			fp = fopen(argv[current_file_index], "r");
		}

		if (NULL == fp)
		{
			fprintf(stderr, "Error opening file: %s\n", strerror(errno));
			continue;
		}
		
		format_line((FILE*)fp, (unsigned int)min_len, (unsigned int)max_len, (char)start_char, (char)end_char, check_is_complex, check_is_not_complex);

		// cleanup
		if (fp != stdin)
		{
			fclose(fp);
		}
	}


	if (argc == optind)
	{
		format_line(stdin, (unsigned int)min_len, (unsigned int)max_len, (char)start_char, (char)end_char, check_is_complex, check_is_not_complex);
	}


	return 0;
}
