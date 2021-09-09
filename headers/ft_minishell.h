/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 13:22:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/09 19:49:18 by edavid           ###   ########.fr       */
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
# define TYPE_OPERATOR	0
# define TYPE_FILENAME	1
# define TYPE_COMMAND	2

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

typedef struct s_std_FDs
{
	char	*inFile;
	char	*outFile;
	char	*errFile;
}	t_std_FDs;

// Command/Filename/Operator
typedef struct s_shell_node
{
	int					type;
	union
	{
		char			*operator;
		char			*filename;
		char			**arguments;
	}	u_data;
	t_std_FDs			FDs;
	struct s_shell_node	*next;
}	t_shell_node;

// Main structure for the project
// 'CommandTable' is a table, each row is an array of simple commands
// A simple command is a NULL terminated str array, ex: "grep" "if" NULL
typedef struct s_minishell
{
	char			*promptStr;
	char			**tokens;
	t_shell_node	*nodes;
	char			*inFile;
	char			*outFile;
	char			*errFile;
	char			**envp;
	t_node_binary	*envpLst;
}	t_minishell;

char		*myreadline(const char *promptMsg);
int			parseCmdLine(t_minishell *mystruct);
int			lexer(t_minishell *mystruct);
bool		isValidCmdLine(char *CmdLine);
int			parser(t_minishell *mystruct);
bool		isCharPartOfSet(char c, char *set);
void		free_mystruct(t_minishell *mystruct);
bool		isValidOperator(char *str);
bool		isValidCommand(t_minishell *mystruct, char *str);
bool		isValidFilename(char *str);
void		init_mystruct(t_minishell *mystruct);

// DEBUG FUNCTIONS
// Print 'mystruct->tokens'.
void		printTokens(t_minishell *mystruct);
// Print 'mystruct->nodes'.
void		printNodes(t_minishell *mystruct);
// Prints a NULL terminated strArr.
void		printStrArr(char **strArr);

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
