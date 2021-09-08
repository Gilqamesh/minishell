/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 13:22:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/08 16:48:19 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

// Library includes
# include "../mylib/mylib.h"
# include "../pipex/headers/pipex.h"
# include <readline/readline.h>
# include <readline/history.h>

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

typedef struct s_node
{
	int		type;
	union
	{
		char	*filename;
		char	**arguments;
	}	u_data;
}	t_node;

// Main structure for the project
// 'CommandTable' is a table, each row is an array of simple commands
// A simple command is a NULL terminated str array, ex: "grep" "if" NULL
typedef struct s_minishell
{
	t_list	*nodes;
	char	*inFile;
	char	*outFile;
	char	*errFile;
}	t_minishell;

char		*myreadline(const char *promptMsg);
void		parseCmdLine(t_minishell *mystruct, char *CmdLine);
char		**lexer(char *str);
bool		isValidCmdLine(char *CmdLine);
void		parser(t_minishell *mystruct, char **tokens);
bool		isCharPartOfSet(char c, char *set);

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
