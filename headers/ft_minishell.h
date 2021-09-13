/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 13:22:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/13 18:45:05 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

// Library includes
# include "../mylib/mylib.h"
# include "../pipex/headers/pipex.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>

// Defines
//
// not supported currently: "&;()"
# define METACHARACTERS " \t\n|<>"
# define WHITESPACES " \t\n"
# define OPERATORS "|<>"
// debugging
# define PRINT_HERE() (ft_printf("line %d, file %s\n", __LINE__, __FILE__))
// type defines for 't_shell_node'
# define TYPE_OPERATOR		0
# define TYPE_SIMPLE_CMD	1

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
