// Command Search and Execution
//
// 1. If the command name contains no slashes, the shell attempts to locate it.
// 		If there exists a shell function by that name, the function is invoked.
// 2. If the name does not match a function, the shell searches for it in the
//		list of shell builtins. If a match is found, that builtin is invoked.
// 3. If the name is neither a function nor a builtin and contains no slashes:
//		- Bash searches each element of $PATH for a directory containing an
//		executable file by that name.
//		- Bash uses a hash table to remember the full pathnames of executable
//		files to avoid multiple PATH searches.
//		- A full search of the directories in $PATH is performed only if the
//		command is not found in the hash table. If the search is unsuccessful,
//		the shell searches for a defined shell function named 'command_not_found_handle'.
//		If that function exists, it is invoked in a separate execution environment
//		with the original command and the original command's argument as its
//		arguments, and the function's exit status becomes the exit status of that
//		subshell.
//		If that function is not defined, the shell prints an error message and
//		returns an exit status of 127.
// 4. If the search is successful, or if the command name contains one or more
//		slashes, the shell executes the named program in a separate execution
//		environment. Argument 0 is set to the name given, and the remaining
//		arguments to the command are set to the arguments supplied, if any.
// 5. If this execution fails because the file is not an executable format, and
//		the file is not a directory, it is assumed to be a shell script and the
//		shell executes it as described in 'Shell Scripts'.
// 6. If the command was not begun asychronously, the shell waits for the command
//		to complete and collects its exit status.


// Command Execution Environment
//
// The shell has an execution environment, which does the following:
// - open files inherited by the shell at invocation, as modified by redirections
// 		supplied to the 'exec' builtin.
// - the current working directory as set by 'cd', 'pushd', or 'popd', or
//		inherited by the shell at invocation.
// - the file creation mode mask as set by 'umask' or inherited from the shell's
//		parent.
// - current traps set by 'trap'.
// - shell parameters that are set by variable assignments or with 'set' or
//		inherited from the shell's parent in the environment.
// - shell functions defined during execution or inherited from the shell's
//		parent in the environment
// - options enabled at invocation (either by default or with command-line
//		arguments) or by 'set'.
// - options enabled by 'shopt'.
// - shell aliases defined with 'alias'.
// - various process IDs, including those of background jobs, the value of $$,
//		and the value of $PPID
//
// When a simple command other than a builtin or shell function is to be executed,
// it is invoked in a separate execution environment that consists of the following.
// Unless otherwise noted, the values are inherited from the shell.
// - the shell's open files, plus any modifications and additions specified by
//		redirections to the command.
// - the current working directory.
// - the file creation mode mask.
// - shell variables and functions marked for 'export', along with variables
//		exported for the command, passed in the Environment.
// - traps caught by the shell are reset to the values inherited from the shell's
//		parent, and traps ignored by the shell are ignored.
//
// A command invoked in this separate environment cannot affect the shell's
// execution environment.
//
// Command substitution, commands grouped with parentheses, and asynchronous commands
// are invoked in a subshell environment that is a duplicate of the shell environment,
// except that traps caught by the shell are reset to the values that the shell
// inherited from its parent at invocation. Builtin commands that are invoked as
// part of a pipeline are also executed in a subshell environment. Changes made
// to the subshell environment cannot affect the shell's execution environment.
//
// Subshells spawned to execute command substitutions inherit the value of the -e
// option from the parent shell. When not in POSIX mode, Bash clears the -e option
// in such subshells.
//
// If a command if followed by a '&' and job control is not active, the default
// standard input for the command is the empty file /dev/null. Otherwise, the
// invoked command inherits the file descriptors of the calling shell as modified
// by redirections.