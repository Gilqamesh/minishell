/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 13:22:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/14 15:28:46 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

// Library includes
# include "../mylib/mylib.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>

// Defines
//
// not supported currently: "&;()"
# define METACHARACTERS " \t\n|<>"
# define WHITESPACES " \t\n"
# define OPERATORS "|<>"
// debugging
# define PRINT_HERE() (ft_printf("line %d, file %s\n", __LINE__, __FILE__))

// Binary Tree ADT to implement Abstract Syntax Tree
// We don't need this for mandatory as we don't have any operator presedence
# ifndef T_BIN_TREE
#  define T_BIN_TREE
typedef struct s_bin_tree
{
	int					type;
	char				*token;
	struct s_bin_tree	*left;
	struct s_bin_tree	*right;
}	t_bin_tree;
# endif

typedef struct s_FD
{
	char					*filename;
	int						fd;
	enum e_redirection_mode	mode;
}	t_FD;

typedef struct s_std_FDs
{
	t_FD	inFile;
	t_FD	outFile;
	t_FD	errFile;
}	t_std_FDs;

typedef struct s_simpleCmd
{
	char				**arguments;
	t_std_FDs			FDs;
	struct s_simpleCmd	*next;
}	t_simpleCmd;

// Main structure for the project
// 'CommandTable' is a table, each row is an array of simple commands
// A simple command is a NULL terminated str array, ex: "grep" "if" NULL
typedef struct s_minishell
{
	char			*promptStr;
	char			**tokens;
	t_simpleCmd		*nodes;
	t_std_FDs		FDs;
	t_obj_lst		*envpLst;
	unsigned char	fgExitStatus;
	t_list			*allocedPointers;
	pid_t			curProcess;
}	t_minishell;

char		*myreadline(const char *promptMsg);
int			parseCmdLine(t_minishell *mystruct);
int			lexer(t_minishell *mystruct);
int			expander(t_minishell *mystruct);
bool		isProperlyNested(char *str);
int			parser(t_minishell *mystruct);
char		isCharPartOfSet(char c, char *set);
char		*isValidOperator(char *str);
bool		isValidCommand(t_minishell *mystruct, char *str);
bool		isValidFilename(char *str);
void		init_mystruct(t_minishell *mystruct);
void		clearStruct(t_minishell *mystruct);
void		ft_simpleCmdadd_back(t_simpleCmd **lst, t_simpleCmd *new);
void		ft_simpleCmdclear(t_simpleCmd **lst, void (*del)(void *));
void		ft_simpleCmddelone(t_simpleCmd *item, void (*del)(void *));
void		ft_simpleCmddel(void *item);
t_simpleCmd	*ft_simpleCmdnew(char **arguments, t_std_FDs *FDs);
int			executor(t_minishell *mystruct);
int			checkSyntax(t_minishell *mystruct);
char		*isValidRedirection(char *str);

typedef struct s_pipex
{
	char	***commands;
	int		nOfCmds;
	int		tmpFd[2];
	int		file[2];
	int		(*pipes)[2];
	bool	(*openPipes)[2];
	bool	isHereDoc;
	int		hereDocPipe[2];
	char	*delimiter;
	t_list	*alloced_lst;
}	t_pipex;

// PIPEX FUNCTIONS
void		error_handler(t_pipex *mystruct, int errcode, char *message);
void		handle_inputFile_firstCmd(t_pipex *mystruct, char *argv[],
				char *envp[]);
int			handle_lastCmd_outputFile(t_pipex *mystruct);
void		destroy_mystruct(t_pipex *mystruct);
void		initialize_mystruct(int argc, char *argv[], char *envp[],
				t_pipex *mystruct);
void		cmd_path(char **cmd, t_obj_lst *lst);
void		closePreviousPipes(t_pipex *mystruct, int upToPipeNum);
void		read_until_delimiter(t_pipex *mystruct);
int			wait_childProcess(void);
void		initOutFile(t_pipex *mystruct, int argc, char **argv);
void		closePipe(t_pipex *mystruct, int pipeNumber, int read_or_write_end);
void		openPipe(t_pipex *mystruct, int pipeNumber);
void		mydup2(t_pipex *mystruct, int fromFd, int toFd);
pid_t		myfork(t_pipex *mystruct);
int			ft_pipex(t_minishell *minishellStruct,
				int argc, char *argv[], char *envp[]);

/*
** Error codes
*/
# define PIPEX_EUSAGE	2	/* Wrong usage */
# define PIPEX_EMALLOC	3	/* Malloc failed */
# define PIPEX_ERR		4	/* Unexpected error */
# define PIPEX_EFOPEN	5	/* Could not open file */
# define PIPEX_EFCLOSE	6	/* Could not close file */
# define PIPEX_EPIPE	7	/* pipe() failed */
# define PIPEX_EFORK	8	/* fork() failed */
# define PIPEX_EDUP2	9	/* dup2() failed */
# define PIPEX_ESTATUS	10	/* Status code error */
# define PIPEX_EEXIT	11	/* Process has not exited properly */
# define PIPEX_ECMD		127	/* Command was not found */

// DEBUG FUNCTIONS
// Prints a NULL terminated strArr.
void		printStrArr(char **strArr);
void		printNodes(t_minishell *mystruct);

// Stack operations
//
// I started to implement these for 'isProperlyNested' function, but I didn't
// use Stack ADT there, so no idea if these will be useful later or not
// Basic stack ADT, uses a linked list to store each element
# ifndef T_STACK
#  define T_STACK
typedef struct s_stack
{
	t_list	*stack;
	int		size;
}	t_stack;
# endif
bool		isStackEmpty(t_stack *sp);
void		pushStack(t_stack *sp, void *content);
void		destroyStack(t_stack *sp);
void		*popStack(t_stack *sp);
t_stack		*createStack(void);

#endif
