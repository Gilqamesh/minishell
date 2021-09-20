/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 13:22:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 19:23:12 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

// Library includes
# include "../mylib/mylib.h"
# include <stdio.h>
# include <limits.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdlib.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>

// Defines
//
# define CMD_PROMPT "hello, please enter a command line: "
// not supported currently: "&;()"
# define METACHARACTERS " \t\n|<>"
# define WHITESPACES " \t\n"
# define OPERATORS "|<>"

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
	bool				isBuiltin;
	struct s_simpleCmd	*next;
}	t_simpleCmd;

// Main structure for the project
// 'pipeLines' is a linked list, each content is a t_simpleCmd linked list
// of a pipeline to be executed in order.
typedef struct s_minishell
{
	char			*promptStr;
	char			**tokens;
	t_simpleCmd		*nodes;
	t_list			*pipeLines;
	t_obj_lst		*envpLst;
	unsigned char	fgExitStatus;
	t_list			*allocedPointers;
	pid_t			lastPID;
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
int			ft_simpleCmdsize(t_simpleCmd *list);
t_simpleCmd	*ft_simpleCmdnew(char **arguments, t_std_FDs *FDs, bool isBuiltin);
int			executor(t_minishell *mystruct);
int			checkSyntax(t_minishell *mystruct);
char		*isValidRedirection(char *str);
char		**ft_strArrDup(char **strArr);
void		printPipelines(t_minishell *mystruct);
t_std_FDs	*copy_FD(t_std_FDs *src);
void 		sighandler(int sig);
int			isOperatorSyntaxErr(char *str);
void		initFD(t_std_FDs *FD);
bool		isStrBuiltin(char *str);
void		executeBuiltin(t_minishell *mystruct, char **commandArgs,
				bool shouldExit);
t_minishell	*getMystruct(t_minishell *mystruct);
// Builtins
int			builtin_echo(char **commandArgs);
int			builtin_export(t_minishell *mystruct, char **commandArgs);
int			builtin_unset(t_minishell *mystruct, char **commandArgs);
int			builtin_cd(t_minishell *mystruct, char **commandArgs);
int			builtin_pwd(t_minishell *mystruct);
int			builtin_env(t_minishell *mystruct);

typedef struct s_pipex
{
	int			nOfCmds;
	int			tmpFd[2];
	int			(*pipes)[2];
	bool		(*openPipes)[2];
	int			hereDocPipe[2];
	char		*delimiter;
	t_list		*alloced_lst;
	t_obj_lst	*envpLst;
	char		**envp;
	t_simpleCmd	*first;
	t_simpleCmd	*last;
}	t_pipex;

// PIPEX FUNCTIONS
void		error_handler(t_pipex *mystruct, int errcode, char *message);
void		handle_inputFile_firstCmd(t_minishell *minishellStruct,
				t_pipex *mystruct);
int			handle_lastCmd_outputFile(t_pipex *mystruct);
void		destroy_mystruct(t_pipex *mystruct);
int			initialize_mystruct(t_minishell *minishellStruct, t_pipex *mystruct,
				t_simpleCmd *pipeLine);
void		cmd_path(char **cmd, t_obj_lst *lst);
void		closePreviousPipes(t_pipex *mystruct, int upToPipeNum);
void		read_until_delimiter(t_pipex *mystruct);
int			wait_childProcess(void);
int			initOutFile(t_pipex *mystruct);
int			closePipe(t_pipex *mystruct, int pipeNumber, int read_or_write_end);
int			openPipe(t_pipex *mystruct, int pipeNumber);
void		mydup2(t_pipex *mystruct, int fromFd, int toFd);
pid_t		myfork(t_pipex *mystruct);
void		ft_pipex(t_minishell *minishellStruct, t_simpleCmd *pipeLine);
void		redirect_stdin(t_pipex *mystruct);
int			terminate_pipex(t_pipex *mystruct, char *message);
t_simpleCmd	*getSimpleCmdIndex(t_simpleCmd *lst, int index);
void		closeFDs(t_pipex *mystruct);
char		*removeLastDirOfPath(char *path);

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

#endif
