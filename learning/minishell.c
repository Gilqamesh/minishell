// 1. expand those words that are not variable assignments and redirections
// 2. redirections are performed
// 3. expansions in each variable assignments


// First order of operations
//
// 1.	Breaks input string into words and operators, obeying the Quoting rules.
// 		These tokens (words/operators) are separated by metacharacters.
//		metacharacters: ' ', '\t', '\n', '|', '&', ';', '(', ')', '<', '>'
// 2.	Parses the tokens into simple and compound commands.
// 3.	Performs the various shell expansions, breaking the expanded tokens into
//		lists of filenames and commands and arguments.
// 4.	Performs any necessary redirections and removes the redirection
//		operators and their operands from the argument list.
// 5.	Executes the command.
// 6.	Optionally waits for the command to complete and collects its exit
//		status.

// Quoting rules:
//
// Escape character: '\'
// 		if not quoted, preserves the literal value of the next character
// Single quotes: '
//		preserves the literal value of all enclosed characters, a single quote
//		may not occur between single quotes
// Double quotes: "
//		preserves the literal value of all enclosed characters, with the
//		exception of: '$', '`', '\' and when history expansion is enabled, '!'

// Environmental variables ($ followed by characters) should expand to their values
// $? should expand to the exit status of the most recently executed foreground
// pipeline.