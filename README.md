# ftcat

Tool to speed up password based attacks by ensuring all words within a wordlist fall within the required password parameters.

```
Usage: ftcat [OPTION]... [FILE]...
format FILE(s) and print to standard output.

With no FILE, or when FILE is -, read standard input.

  -1            set default options for WPA
  -2            set default options for "Password must meet complexity requirements"
  -3            set default options for "Password does not meet complexity requirements"
 
  -m            min word length
  -M            max word length
  -s            start character (ascii range)
  -e            end character (ascii range)
  -h            display this help and exit

Examples:
  ftcat foo bar  Formats foo's contents, then bar's contents.
  ftcat          Copy standard input to standard output.

```
