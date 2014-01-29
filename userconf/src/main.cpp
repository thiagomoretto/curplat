/***************************************************************************
 *            main.cpp
 *
 *  Thu Nov 29 16:12:00 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <panel.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define T_LINES LINES
#define T_COLS COLS

#ifndef VERSION
	#define VERSION "0.4-devel"
#endif

#define C_BACKGROUND 1
#define C_WINDOW 2
#define C_TITLE 3
#define C_MENU_S 4
#define C_BUTTON_S 5
#define C_STDSCR_T 6
#define C_LINEBOX 7
#define C_DIALOG_C 8
#define C_SUCCESS 9
#define C_RED C_MENU_S

#include "dialog.h"
#include "button.h"
#include "contentbox.h"
#include "linebox.h"
#include "fileman.h"
#include "menu.h"
#include "grid.h"

void quit (void)
{
	endwin ();
	exit(1);
}

/*****************************************
 * Funcoes de auxilio a manipulacao dos arquivos
 ****************************************/


/*****************************************
 * Funcoes auxiliares p/ as janelas
 ****************************************/
typedef struct {
        int begin_x, begin_y;
} dot;

dot begin_xy ( int w, int h, int offset_vertical, int offset_horizontal )
{
        dot p;
        p.begin_x = ((T_COLS - w) /2 - 1) + offset_horizontal;
	    p.begin_y = ((T_LINES - h)/2 - 1) + offset_vertical;
		if (p.begin_x < 0) p.begin_x = 1;
		if (p.begin_y < 0) p.begin_y = 1;
        return p ;
}

/*****************************************
 * Caixas de alerta
 ****************************************/
void alert (char *msg , int dx)
{
	int ret ;
	DIALOG *aux;
	BUTTON *botao_aux ;
	dot p;
	
	p = begin_xy (dx, 7, 0 ,0);
	aux = new_dialog (7, dx, p.begin_y, p.begin_x, 4);
	dialog (aux);
	show_dialog (aux);

	wattron (aux->dialog_window , COLOR_PAIR(4));
	mvwprintw (aux->dialog_window ,1, 1, msg);
	wattroff (aux->dialog_window , COLOR_PAIR(4));

	botao_aux = new_button (aux, 3, 15, aux->begin_y + 3, p.begin_x + ((dx - 15)/2) , 2, C_BUTTON_S );
	setlabel (botao_aux, "OK");
	print_button (botao_aux);
	
	refresh_dialog (aux);
	while (1){
		ret = run_button (botao_aux);
		if (ret == 0) {
			hide_dialog (botao_aux->dialog_button);
			hide_dialog (aux);
			del_button (botao_aux);
			del_dialog (aux);
			return ;
		}
	}
}


void alert2 (char *msg, int dx, int pair)
{
	DIALOG *alert;
	dot p;
	p = begin_xy (dx, 4, 0, 0);
	alert = new_dialog (4, dx, p.begin_y, p.begin_x, pair);
	enable_shadow (alert, 0);
	dialog (alert);
	mvwprintw (alert->dialog_window, 1, 1, msg);
	mvwprintw (alert->dialog_window, 2, 1, "Pressione qualquer tecla para continuar.");
	refresh_dialog (alert);
	getch ();
	werase (alert->dialog_window);
	hide_dialog	(alert);
	del_dialog (alert);
}

bool confirmbox (char *msg, int dx, int pair)
{
	int ret , tab = 1;
	DIALOG *confirm;
	BUTTON *botao_ok, *botao_cancelar;
	dot p;
	
	p = begin_xy (dx, 7, 0 ,0);
	confirm = new_dialog (7, dx, p.begin_y, p.begin_x, pair);
	dialog (confirm);
	show_dialog (confirm);

	wattron (confirm->dialog_window , COLOR_PAIR(pair));
	mvwprintw (confirm->dialog_window ,1, 1, msg);
	wattroff (confirm->dialog_window , COLOR_PAIR(pair));

	botao_ok = new_button (confirm, 3, 10, confirm->begin_y + 3, p.begin_x + ((dx - 10)/2)/2 , C_MENU_S, C_BUTTON_S );
	setlabel (botao_ok, "OK");
	botao_cancelar = new_button (confirm, 3, 10, confirm->begin_y + 3, 15 + p.begin_x + ((dx - 10)/2)/2 , C_MENU_S, C_BUTTON_S );
	setlabel (botao_cancelar, "Cancelar");

	print_button (botao_ok);
	print_button (botao_cancelar);
	refresh_dialog (confirm);
	
	while (true){
		if (tab == 1)
		{
			ret = run_button (botao_ok);
			if (ret == 0) {
				hide_dialog (botao_ok->dialog_button);
				hide_dialog (botao_cancelar->dialog_button);
				hide_dialog (confirm);
				del_buttons (2, botao_ok ,botao_cancelar);
				del_dialog (confirm);
				return true;
			}
			tab = 2;
		}
		if (tab == 2)
		{
			ret = run_button (botao_cancelar);
			if (ret == 0) {
				hide_dialog (botao_ok->dialog_button);
				hide_dialog (botao_cancelar->dialog_button);
				hide_dialog (confirm);
				del_buttons (2, botao_ok ,botao_cancelar);
				del_dialog (confirm);
				return false;
			}
			tab = 1;
		}
	}
}

/*****************************************
 * Menus (NecessÃ¡rio recriar o menu)
 *****************************************/
int open_menu ()
{
	menu dados; 
	dot p; int opt;
	WINDOW * menubox;
	menu_position mp;
	
	p = begin_xy (50, 6, 0, 0);	
	dados.nopt = 4;
	dados.x	= p.begin_x, dados.y = p.begin_y;
	dados.width	= 50;
	dados.dpair	= 2, dados.hkpair 	= 3,  dados.spair 	= 4, dados.color_pair = 2;
				
	dados.options = alloc_opt_c (dados.nopt, dados.width);
	dados.hotkeys = new int [dados.nopt];

	strcpy (dados.options[0], "Inserir"  );	dados.hotkeys [0] = 0;
	strcpy (dados.options[1], "Buscar"   );	dados.hotkeys [1] = 0;
	strcpy (dados.options[2], "Remover"  );	dados.hotkeys [2] = 1;	
	strcpy (dados.options[3], "Sair"     );	dados.hotkeys [3] = 3;
	return buildmenu (menubox , &dados, &mp);
}

/*****************************************
 * Caixas de conteudo/texto (contentbox)
 ****************************************/
CONTENTBOX * create_mensagem ( void )
{
	dot p;
	CONTENTBOX *mensagem;
	p = begin_xy (60, 6, -10, 0);
	mensagem = new_contentbox (6, 60, p.begin_y, p.begin_x, 2, 3);
	settitle (mensagem, "Configuracao de usuarios do sistema");
	setcontent (mensagem, "\nManutecao dos usuarios do sistema, insercao, remocao,\nmodificacao, visualizacao de dados", 3, 60);
	enable_shadow(mensagem->dialog_contentbox, 0);
	return mensagem;
}
void open_mensagem ( CONTENTBOX *mensagem )
{
	show_contentbox (mensagem);
}

/*****************************************
 * Formulario (FORM1 INSERCAO)
 ****************************************/
DIALOG * create_form(int nlines, int cols)
{
	DIALOG *form1;
	dot p;
	p = begin_xy (cols, nlines, 0, 0);
	form1 = new_dialog (nlines, cols, p.begin_y, p.begin_x, 2);
	enable_shadow (form1, 0);
	dialog (form1);
	refresh_dialog (form1);
	hide_dialog (form1);
	return form1;
}

void open_form ( DIALOG *form1 )
{
	int ret = 0, tab = 1, loop = 0, n_alloc, sucesso = 0;
	WINDOW *form1_window;
	DIALOG *save_dialog;
	LINEBOX *line_fullname, *line_login, *line_dir, *line_group, *line_password, *line_retyped;
	BUTTON * botao_clear, *botao_cancelar, *botao_salvar;
	USR *users;
	dot p;
	char *commandline_buffer = new char [100];
	p = begin_xy (70, 4, 0, 0);
	form1_window = request_window(form1);

	wattron (form1_window, COLOR_PAIR (3));
	mvwprintw (form1_window, 1, 1, "Modo de inserção de usuário");
	wattroff (form1_window, COLOR_PAIR (3)); 
	
	mvwprintw (form1_window, 3, 1, " Insira o nome completo, login  de acesso, dire");
	mvwprintw (form1_window, 4, 1, " torio principal e grupo para completar o cadas");
	mvwprintw (form1_window, 5, 1, " tro de um novo usuário ao seu sistema.");

	botao_salvar = new_button (form1, 3, 14, form1->begin_y + 16, form1->begin_x + 2, 4, 5);
	setlabel (botao_salvar, "Salvar(F2)");
	print_button (botao_salvar);
	
	botao_cancelar = new_button (form1, 3, 14, form1->begin_y + 16 , form1->begin_x + 18, 4, 5);
	setlabel (botao_cancelar, "Voltar(F3)");
	print_button (botao_cancelar);

	botao_clear = new_button (form1, 3, 14, form1->begin_y + 16, form1->begin_x + 34, 4, 5);
	setlabel (botao_clear, "Limpar");
	print_button (botao_clear);

	/* CONSTRUCOES DOS EDITORES DE LINHAS */
	wattron (form1_window, COLOR_PAIR (3));
	mvwprintw (form1_window, 7, 1, "      Full name:");
	mvwprintw (form1_window, 8, 1, "          Login:");
	mvwprintw (form1_window, 9, 1, "       Home dir:");
	mvwprintw (form1_window, 10, 1,"  Default Group:");
	wattroff (form1_window, COLOR_PAIR (3));
	mvwprintw (form1_window, 12, 1, " Set a password and re-type:");
	wattron (form1_window, COLOR_PAIR (3));
	mvwprintw (form1_window, 13, 1,"       Password:");
	mvwprintw (form1_window, 14, 1,"        Re-type:");
	wattroff (form1_window, COLOR_PAIR (3));
	
	/*DEFININDO OS EDITORES*/
	/*							   l, c, y, x, cp, bor, max */
	line_fullname 	= new_linebox( form1,  1, 30, form1->begin_y + 7, form1->begin_x + 18,  7, 0, 29);
	line_login 		= new_linebox( form1,  1, 30, form1->begin_y + 8,  form1->begin_x + 18,  7, 0, 29);
	line_dir 		= new_linebox( form1,  1, 30, form1->begin_y + 9, form1->begin_x + 18, 7, 0, 29);
	line_group 		= new_linebox( form1,  1, 30, form1->begin_y + 10, form1->begin_x + 18, 7, 0, 29);
	valid_chars (line_fullname , "0123456789abcdefghijklmnopqrstuvywxzQWERTYUIOPASDFGHJKLÃ‡ZXCVBNM -");
	valid_chars (line_login    , "0123456789abcdefghijklmnopqrstuvywxzQWERTYUIOPASDFGHJKLÃ‡ZXCVBNM");
	valid_chars (line_dir      , "0123456789abcdefghijklmnopqrstuvywxzQWERTYUIOPASDFGHJKLÃ‡ZXCVBNM-_/");
	valid_chars (line_group    , "0123456789abcdefghijklmnopqrstuvywxzQWERTYUIOPASDFGHJKLÃ‡ZXCVBNM-_");

	line_password 	= new_linebox( form1,  1, 30, form1->begin_y + 13, form1->begin_x + 18, 7, 0, 16);
	line_retyped 	= new_linebox( form1, 1, 30, form1->begin_y + 14, form1->begin_x +18, 7, 0, 16);
	valid_chars (line_password, "0123456789abcdefghijklmnopqrstuvywxzQWERTYUIOPASDFGHJKLÃ‡ZXCVBNM");
	valid_chars (line_retyped , "0123456789abcdefghijklmnopqrstuvywxzQWERTYUIOPASDFGHJKLÃ‡ZXCVBNM");
	
	change_type (line_password, PASSWORD );
	change_type (line_retyped, PASSWORD );
	
	print_linebox (line_fullname );
	print_linebox (line_login );
	print_linebox (line_dir );
	print_linebox (line_group );
	print_linebox (line_password );
	print_linebox (line_retyped );

	/*INICIANDO INTERACAO DO FORM*/
	refresh_dialog	(form1);
	wrefresh (stdscr);
	wrefresh (form1_window);
	tab = 1;
	
	while (1)
	{
		if (tab == 1) {
			ret = run_linebox (line_fullname);
			if (ret == -1) tab--;
			if (ret ==  1) tab++;
			if (ret == KEY_F(2)) { tab = 7; }
			if (ret == KEY_F(3)) { tab = 8; }
			}

		if (tab == 2) {
			ret = run_linebox (line_login);
			if (ret == -1) tab--;
			if (ret ==  1) tab++;
			if (ret == KEY_F(2)) { tab = 7; }
			if (ret == KEY_F(3)) { tab = 8; }
			}
		if (tab == 3) {
			ret = run_linebox (line_dir);
			if (ret == -1) tab--;
			if (ret ==  1) tab++;
			if (ret == KEY_F(2)) { tab = 7; }
			if (ret == KEY_F(3)) { tab = 8; }
			}
		if (tab == 4) {
			ret = run_linebox (line_group);
			if (ret == -1) tab--;
			if (ret ==  1) tab++;
			if (ret == KEY_F(2)) { tab = 7; }
			if (ret == KEY_F(3)) { tab = 8; }
			}
		if (tab == 5) {
			ret = run_linebox (line_password);
			if (ret == -1) tab--;
			if (ret ==  1) tab++;
			if (ret == KEY_F(2)) { tab = 7; }
			if (ret == KEY_F(3)) { tab = 8; }
			}
		if (tab == 6) {
			ret = run_linebox (line_retyped);
			if (ret == -1) tab--;
			if (ret ==  1) tab++;
			if (ret == KEY_F(2)) { tab = 7; }
			if (ret == KEY_F(3)) { tab = 8; }
			}	
		if (tab == 7) {
			ret = run_button (botao_salvar);
			if (ret == -1) tab--;
			if (ret ==  0) { 
				
				n_alloc = get_nalloc();
					
				if (n_alloc <= 0)
					n_alloc = 1;
				

				
				/**BACK-END**/
				users = new_data (n_alloc);
				users = load_data (n_alloc);

				/*VERIFICA DADOS*/
				/*VERIFICAO DE PREENCHIMENTO*/				
				if (line_fullname->nchar <= 0)
					alert2 ("Erro: Informe o nome completo do usuário.", 55, C_RED);
				else if (line_login->nchar <= 0)
					alert2 ("Erro: Informe o login do usuario.", 55, C_RED);
				else if (line_dir->nchar <= 0)
					alert2 ("Erro: Informe o diretório home.", 55, C_RED);
				else if (line_group->nchar <= 0)
					alert2 ("Erro: Informe o grupo.", 55, C_RED);
				else 
					sucesso ++;
				
				if (sucesso == 1)
				{
					/*VERIFICACAO DA SENHA*/
					if (line_password->nchar <= 0  && line_retyped->nchar <= 0) {
						// Pede confirmaçao se pode cadastrar usuario sem senha
						if (confirmbox("Inserir usuário sem senha?", 40, C_RED)) sucesso ++;
						else sucesso --;	
					} else if (strcmp(line_password->buffer, line_retyped->buffer) != 0) {
						alert2 ("Erro: A senha digitada é diferente da redigitada.", 55, C_RED);
						reset_linebox (line_password);
						reset_linebox (line_retyped);
						tab = 5; continue;
					} else 
						sucesso ++;
				}
				
				if (sucesso == 2)
				{
					/**INSERE O USUARIO**/
					/**CHAMA AS FUNCOES PARA ESTE FIM**/
					save_dialog = new_dialog ( 4, 70, p.begin_y, p.begin_x, 4);
					mvwprintw (save_dialog->dialog_window, 1, 1, "Salvando informaçoes do usuário...");
					dialog (save_dialog);
					refresh_dialog (save_dialog);
					sleep (1);
					mvwprintw (save_dialog->dialog_window, 2, 1, "Informacoes gravadas. Pressiona qualquer tecla para continuar.");
					refresh_dialog (save_dialog);
					getch ();
					hide_dialog (save_dialog);
					del_dialog (save_dialog);
					
					strcpy (users[n_alloc-1].fullname, line_fullname->buffer);
					strcpy (users[n_alloc-1].login, line_login->buffer);
					strcpy (users[n_alloc-1].dir, line_dir->buffer);
					strcpy (users[n_alloc-1].group, line_group->buffer);
					users [n_alloc-1].id = n_alloc;
					save_data (users, n_alloc );
					
	#if 0
					/*GROUPADD*/
					sprintf (commandline_buffer, "groupadd %s 2>&1 > /dev/null", 
						line_group->buffer);
					system (commandline_buffer);
					
					/*USERADD*/
					sprintf (commandline_buffer,"useradd -g %s -d %s %s 2>&1 > /dev/null", 
						line_group->buffer, line_dir->buffer, line_login->buffer);
					
					system (commandline_buffer);
					/*PASSWD*/
					sprintf (commandline_buffer, "echo %s | passwd %s --stdin 2>&1 > /dev/null",
						line_password->buffer, line_login->buffer);
					system (commandline_buffer);
	#endif
	
					put_nalloc (++n_alloc);
					
					mvwprintw (stdscr, 5, 5, "%d", n_alloc);
					wrefresh(stdscr);
					
					/**************
					END SAVING
					**************/
				}
				sucesso = 0;
			  }
			if (ret ==  1) tab++;
			if (ret == KEY_F(3)) { tab = 8; }
		}

		if (tab == 8) {
			ret = run_button (botao_cancelar);
			if (ret == -1) tab--;
			if (ret ==  0) {
					hide_dialog(form1);
					del_dialog(botao_salvar->dialog_button);
					del_dialog(botao_cancelar->dialog_button);
					del_dialog(botao_clear->dialog_button);
					del_dialog(line_fullname->dialog_linebox);
					del_dialog(line_login->dialog_linebox);
					del_dialog(line_dir->dialog_linebox);
					del_dialog(line_group->dialog_linebox);
					del_dialog(line_password->dialog_linebox);
					del_dialog(line_retyped->dialog_linebox);
					free_linebox (line_fullname), free_linebox (line_login), free_linebox (line_dir), free_linebox (line_group);
					free_linebox (line_password), free_linebox (line_retyped);
					return; 
				};
			if (ret ==  1) tab++;
			if (ret == KEY_F(2)) { tab = 7; }
			if (ret == KEY_F(3)) { tab = 8; }
		}

		if (tab == 9) {
			ret = run_button (botao_clear);
			if (ret == -1) tab--;
			if (ret ==  0) { 
				reset_linebox (line_fullname);
				reset_linebox (line_login);
				reset_linebox (line_dir);
				reset_linebox (line_group);
				reset_linebox (line_password);
				reset_linebox (line_retyped);
				};
			if (ret ==  1) tab++;
			if (ret == KEY_F(2)) { tab = 7; }
			if (ret == KEY_F(3)) { tab = 8; }
		}
		
		mvprintw (2,2, "%d", loop);
		loop ++;
		
		if (tab == 10) tab = 1;
		if (tab ==  0) tab = 9;
	}
}

/*****************************************
 * REMOVE O USUÁRIO
 ****************************************/
void remove_user (USR *users, int n_alloc, int n)
{
	if (n_alloc > 1) {
		strcpy (users[n].fullname, 	users[n_alloc-2].fullname);
		strcpy (users[n].login , 	users[n_alloc-2].login);
		strcpy (users[n].dir , 		users[n_alloc-2].dir);
		strcpy (users[n].group , 	users[n_alloc-2].group);	
	}

	save_data (users, n_alloc - 1);
	put_nalloc (n_alloc - 1);
	
	users = load_data (n_alloc - 1);
}

#define REFRESH_GRID \
					remove_user (users, n_alloc, gridbox->active);\
					setnreg (gridbox, n_alloc-2);\
										\
					for (i=0; i<n_alloc-2; i++) {\
						tmpstr = (char *)malloc\
						(strlen(users[i].login)+strlen(users[i].fullname)+strlen(users[i].group)+strlen(users[i].dir)+15);\
						sprintf (tmpstr, "%d %s\t%s\t%s\t%s", i, users[i].login, users[i].group, users[i].dir, users[i].fullname);\
						setregline (gridbox, tmpstr, i);\
						free (tmpstr);\
					}\
					wclear (gridbox->grid_window);\
					print_grid (gridbox);\
					wrefresh (gridbox->grid_window);\
					n_alloc--;\
					gridbox->active = 1;

/*****************************************
 * Formulario (FORM2 LISTAGEM)
 ****************************************/
void change_password (int active);
					
void open_form_list ( DIALOG *form_list )
{
	int ret = 0, tab = 1, i ,tab2 = 1;
	BUTTON *botao_sair, *botao_remover, *botao_definir_senha, *botao_aux;
	BUTTON *botao_ok, *botao_cancelar;
	WINDOW *form_list_window;
	int n_alloc = get_nalloc();
	char *tmpstr;
	USR *users;
	GRID *gridbox;
	DIALOG *remover, *nova_senha, *aux;
	dot p;
	
	users = load_data (n_alloc);
	
	if (users == NULL) {
		endwin();
		exit (EXIT_FAILURE);
	}
	
	form_list_window = request_window (form_list);

	mvwprintw (stdscr, 4, 5, "%d", n_alloc);
	wrefresh(stdscr);
	
	gridbox = new_grid (form_list_window, 8, 68, form_list->begin_y + 7, form_list->begin_x + 1, 2, 4 );
	if (n_alloc > 0)
	{
		setnreg (gridbox, n_alloc-1);

		for (i=0; i<n_alloc-1; i++) {
			tmpstr = (char *)malloc(
				strlen(users[i].login)+
				strlen(users[i].fullname)+
				strlen(users[i].group)+
				strlen(users[i].dir)+15);
			sprintf (tmpstr, "%s\t%s\t%s\t%s", users[i].login, users[i].group, users[i].dir, users[i].fullname);
			setregline (gridbox, tmpstr, i);
			free (tmpstr);
		}

		print_grid (gridbox);
	}
	
	wattron (form_list_window, COLOR_PAIR (3));
	mvwprintw (form_list_window, 1, 1,  " Listando usuários");
	mvwprintw (form_list_window, 6, 1,  "Login");
	mvwprintw (form_list_window, 6, 16, " Grupo");
	mvwprintw (form_list_window, 6, 31, " Diretorio");
	mvwprintw (form_list_window, 6, 46, " Nome");
	wattroff (form_list_window, COLOR_PAIR (3)); 
	
	mvwprintw (form_list_window, 3, 1, "Selecione o usuario para altera a senha ou para ser removido");
	mvwprintw (form_list_window, 4, 1, "do seu sistema.");

	botao_sair = new_button (form_list, 3, 14, form_list->begin_y + 16, form_list->begin_x + 2, 4, 5);
	setlabel (botao_sair, "Cancelar");
	print_button (botao_sair);
	
	botao_remover = new_button (form_list, 3, 14, form_list->begin_y + 16, form_list->begin_x + 18, 4, 5 );
	setlabel (botao_remover, "Remover");
	print_button (botao_remover);

	botao_definir_senha = new_button (form_list, 3, 14, form_list->begin_y + 16, form_list->begin_x + 34, 4, 5 );
	setlabel (botao_definir_senha, "Nova senha");
	print_button (botao_definir_senha);
	
	show_dialog (form_list);
	refresh_dialog (form_list);
	
	tab = 1;
	while (1) {
		/****
		 * GRID 
		 * */		
		if (tab == 1) {
			if (n_alloc-1 >	0) {
				ret = run_grid(gridbox);
				if (ret == RET_DELETE)
					{
						if (confirmbox("Você tem certeza que deseja remover este usuário?", 52, C_RED)) {
							REFRESH_GRID
						}
					}
				else if (ret == -2) tab = 2; /*NEXT*/
			} else tab = 2;
		}

		/****
		 * BOTAO SAIR
		 * */		
		if (tab == 2) {
			ret = run_button (botao_sair);
			if (ret == -1) tab--;
			if (ret ==  0) {
				hide_dialog (form_list);
				hide_dialog (botao_sair->dialog_button);
				del_dialog (botao_sair->dialog_button);
				hide_dialog (botao_remover->dialog_button);
				del_dialog (botao_remover->dialog_button);
				hide_dialog (botao_definir_senha->dialog_button);
				del_dialog (botao_definir_senha->dialog_button);
				return ;
			};
			if (ret ==  1) tab++;
		}

		/**
		 *
		 * BOTAO REMOVER 
		 * 
		 * ***/
		if (tab == 3) {
			ret = run_button (botao_remover);
			if (ret == -1) tab --;
			if (ret ==  0) {
				
			  if (n_alloc-1 < 1) {
			 	alert (" Não há nenhum registro para remover!", 40);
 			  }
			  else
 			  {
				p = begin_xy (50, 7, 0 ,0);
				remover = new_dialog (7, 50, p.begin_y, p.begin_x, 4);
				dialog (remover);
				show_dialog (remover);

				wattron (remover->dialog_window , COLOR_PAIR(4));
				mvwprintw (remover->dialog_window ,1, 1, "Deseja realmente remover este usuário?");
				wattroff (remover->dialog_window , COLOR_PAIR(4));
				refresh_dialog (remover);
				
				botao_ok =
				new_button (remover, 3, 10, remover->begin_y + 3, remover->begin_x + 14, 4, 5 );
				setlabel (botao_ok, "Sim");
				print_button (botao_ok);
		
				botao_cancelar = 
				new_button (remover, 3, 10, remover->begin_y + 3, remover->begin_x + 25, 4, 5 );
				setlabel (botao_cancelar, "Não");
				print_button (botao_cancelar);
				
				while (1)
				{
					/******* SIM ********/
					if (tab2 == 1) {
						/** ATIVA SOMENTE COM N_ALLOC > 0 **/
						ret = run_button(botao_ok);
						if (ret ==  1) tab2 = 2;
						if (ret == -1) tab2 = 2;
						if (ret ==  0) {
				
							REFRESH_GRID

							hide_dialog (botao_cancelar->dialog_button);
							hide_dialog (botao_ok->dialog_button);
							hide_dialog (remover);
							del_dialog (remover);
							del_dialog (botao_cancelar->dialog_button);
							del_dialog (botao_ok->dialog_button);
							break;
						}
					}

					/******** NAO *******/
					if (tab2 == 2) {
						ret = run_button(botao_cancelar);
						if (ret == -1) tab2 = 1;
						if (ret ==  0) {
							hide_dialog (botao_cancelar->dialog_button);
							hide_dialog (botao_ok->dialog_button);
							hide_dialog (remover);
							del_dialog (remover);
							del_dialog (botao_cancelar->dialog_button);
							del_dialog (botao_ok->dialog_button);
							break;

						}
						if (ret == 1) tab2 = 1;
					}
					if (tab2 == 3) tab2 == 1;
					if (tab2 == 0) tab2 == 2;
				}
			  }/*else (if n_nalloc > 0) 	*/
			}
			if (ret == 1) tab++;
		}

		/**
		 *
		 * BOTAO NOVA SENHA
		 * 
		 * ***/
		if (tab == 4) {
			ret = run_button (botao_definir_senha);
			if (ret == -1) tab --;
			if (ret ==  0) {
				/*define senha*/
				if (n_alloc-1 < 1) {
					alert (" Não há nenhum registro para alterar senha", 45);
				} else
					change_password (gridbox->active);
			}
			if (ret == 1) tab++;
		}
		
		if (tab == 5) tab = 1;
		if (tab == 0) tab = 4;
		
		i++;		
	}		
	
}

void change_password (int active)
{
	int tab = 1, ret, sucesso = 0;
	DIALOG *passwd_d;
	BUTTON *confirm, *cancela;
	LINEBOX *newpasswd, *retyped;
	dot p;
	
	p = begin_xy (40, 10, 0, 0);
	passwd_d = new_dialog( 10, 40, p.begin_y, p.begin_x, C_WINDOW);
	enable_shadow (passwd_d, 0);
	dialog (passwd_d);
	refresh_dialog (passwd_d);
	
	wattron (passwd_d->dialog_window, COLOR_PAIR (C_TITLE));
	mvwprintw (passwd_d->dialog_window, 1, 1, " Alterando senha do usuario");
	wattroff (passwd_d->dialog_window, COLOR_PAIR (C_TITLE));
	wattron (passwd_d->dialog_window, COLOR_PAIR (0));
	mvwprintw (passwd_d->dialog_window, 3, 1, " Nova senha:");
	mvwprintw (passwd_d->dialog_window, 4, 1, "   Redigite:");
	wattron (passwd_d->dialog_window, COLOR_PAIR (0));
	wrefresh (passwd_d->dialog_window);
	
	confirm = new_button (passwd_d, 3, 15, p.begin_y+6, p.begin_x +5, C_MENU_S, C_BUTTON_S);
	setlabel (confirm, "Confirma");
	cancela = new_button (passwd_d, 3, 15, p.begin_y+6, p.begin_x +21, C_MENU_S, C_BUTTON_S);
	setlabel (cancela, "Cancelar");

	newpasswd = new_linebox(passwd_d , 1, 24, p.begin_y+3, p.begin_x+14, C_LINEBOX, 0, 10 );
	valid_chars (newpasswd , "0123456789abcdefghijklmnopqrstuvywxzQWERTYUIOPASDFGHJKLAZXCVBNM");
	change_type (newpasswd, PASSWORD);

	retyped = new_linebox(passwd_d , 1, 24, p.begin_y+4, p.begin_x+14, C_LINEBOX, 0, 10 );
	valid_chars (retyped , "0123456789abcdefghijklmnopqrstuvywxzQWERTYUIOPASDFGHJKLAZXCVBNM");
	change_type (retyped, PASSWORD);

	print_linebox (newpasswd);
	print_linebox (retyped);
	print_button (confirm);
	print_button (cancela);
	
	while ( true ) {
		if (tab == 1) {
			ret = run_linebox (newpasswd);
			tab += ret;
		}
		if (tab == 2) {
			ret = run_linebox (retyped);
			tab += ret;
		}
		if (tab == 3) {
			ret = run_button (confirm);
			tab += ret;
			if (ret == 0) {
				/*Ativa*/
				if (strcmp (newpasswd->buffer, retyped->buffer) != 0) {
					
					alert2 ("A senha digitada é diferente da redigitada.", 50, C_RED);
					reset_linebox (newpasswd);
					reset_linebox (retyped);
					tab = 1;
					
				} else {
					
					sucesso = 1;
					if (newpasswd->nchar < 1)
							if(!confirmbox ("Deseja setar usuario sem senha?", 50, C_RED))
								sucesso = 0;
						
					if (sucesso == 1) {
						alert2 ("Senha alterada.", 50, C_SUCCESS);
					
						hide_dialog (passwd_d);
						del_buttons (2, confirm, cancela);
						del_linebox (newpasswd);
						del_linebox (retyped);
						return ;	
					}
				}
			}
		}
		
		if (tab == 4) {
			ret = run_button (cancela);
			tab += ret;
			if (ret == 0) {
				/*Ativa*/
				hide_dialog (passwd_d);
				del_buttons (2, confirm, cancela);
				del_linebox (newpasswd);
				del_linebox (retyped);
				return;
			}
		}
		if (tab == 5) tab = 1;
		if (tab == 0) tab = 3;
	}
	
	getch ();
}

/*****************************************
 * Formulario (FORM3 SEARCH)
 ****************************************/
void open_form_remove ( DIALOG *form )
{
	int ret = 0, tab = 1;
	BUTTON *botao_ok, *botao_cancelar;
	WINDOW *form_window;
	form_window = request_window (form);
	
	wattron (form_window, COLOR_PAIR (3));
	mvwprintw (form_window, 1, 1, "Removendo usuário");
	wattroff (form_window, COLOR_PAIR (3)); 
	
	mvwprintw (form_window, 3, 1, " Informe parte do nome ou login do usuário");
	mvwprintw (form_window, 4, 1, " para procura e remover o usuário do sistema.");
	
	wattron (form_window, COLOR_PAIR (3));
	mvwprintw (form_window, 6, 1, "   Login/user name:");
	mvwprintw (form_window, 7, 1, " Part of Full name:");
	wattroff (form_window, COLOR_PAIR (3)); 
	mvwprintw (form_window, 9, 1, " [ ] Usar expressões regulares?");
	
	botao_ok = new_button (form, 3, 14, form->begin_y + 11, form->begin_x + 2, 4, 5);
	setlabel (botao_ok, "OK");
	print_button (botao_ok);

	botao_cancelar = new_button (form, 3, 14, form->begin_y + 11, form->begin_x + 34, 4, 5);
	setlabel (botao_cancelar, "Cancelar");
	print_button (botao_cancelar);
	
	show_dialog (form);
	refresh_dialog (form);
	
	while (1) {
		if (tab == 1) {
			ret = run_button (botao_ok);
			if (ret == -1) { tab--;};
			if (ret ==  0) {};
			if (ret ==  1) { tab++;};
		}

		if (tab == 2) {
			ret = run_button (botao_cancelar);
			if (ret == -1) { tab--;};
			if (ret ==  0) {
					hide_dialog (form);
					del_dialog (botao_ok->dialog_button);
					del_dialog (botao_cancelar->dialog_button);
					return ;
				};
			if (ret ==  1) { tab++; };
		}
		
		if (tab == 3) tab = 1;
		if (tab == 0) tab = 2;
	}
	
}


/*****************************************
 * Main
 ****************************************/
int main (void)
{
	int opt = -1;
	WINDOW *horario;

	initscr ();
	start_color ();
	curs_set (0);
	cbreak();
	intrflush (stdscr, true);
	
	pid_t child;

#if 0
	child = fork ();
	if (child == 0)
	{
		struct tm *local;
		time_t t;
		stdscr = initscr ();
		horario = subwin (stdscr, 1, 60, 0, 0);
						
		while (1)
		{
			t = time (NULL);
			local = localtime( &t );
			
			//mvwprintw (stdscr, 0, 0, "%02d/%02d/%02d %02d:%02d:%02d",
				//local->tm_mday, local->tm_mon+1, local->tm_year+1900,
				//local->tm_hour, local->tm_min, local->tm_sec
				//);
			opt = local->tm_sec;
			cout << opt;
			
			wrefresh (horario);
			sleep(1);
		}
		endwin ();
		exit (1);
	}
#endif
	
#ifdef ONLY_ROOT
	dot p;
	DIALOG *alert_user;
	uid_t user_id;
	user_id = getuid ();
	init_pair (9, COLOR_WHITE, COLOR_RED);
	
	p = begin_xy ( 50, 3, 0, 0 );
	if (user_id != 0) {
		alert_user = new_dialog ( 3, 50, p.begin_y, p.begin_x , 9);
		dialog (alert_user);
		mvwprintw (alert_user->dialog_window, 1, 1, "Permissão negada, somente root." );
		show_dialog (alert_user);
		getch ();
		endwin ();
		exit (EXIT_FAILURE);
	} 
#endif

	CONTENTBOX 	*mensagem;
	DIALOG 		*form1, *form_list, *form_remove;

	stdscr = newwin (T_LINES, T_COLS, 0, 0);
	intrflush (stdscr, true);

	mensagem    = create_mensagem ();
	form1	    = create_form (20, 50);
	form_list   = create_form (20, 70);
	form_remove = create_form (15, 50);

	keypad (stdscr , true);
	curs_set (false);
	
	init_pair (C_BACKGROUND, COLOR_WHITE, COLOR_BLUE); 	/*BACKGROUND*/
	init_pair (C_WINDOW, COLOR_BLACK, COLOR_WHITE);	/*JANELA*/
	init_pair (C_TITLE, COLOR_RED, COLOR_WHITE); 		/*TITULO*/
	init_pair (C_MENU_S, COLOR_WHITE, COLOR_RED); 		/*MENU SELECIONADO*/
	init_pair (C_BUTTON_S, COLOR_WHITE, COLOR_BLUE);		/*BOTAO SELECIONADO*/
	init_pair (C_STDSCR_T, COLOR_CYAN, COLOR_BLUE);		/*STDSCR TEXT*/
	init_pair (C_LINEBOX, COLOR_RED, COLOR_WHITE);		/*LINEBOX*/
	init_pair (C_DIALOG_C, COLOR_WHITE, COLOR_GREEN);	/*DIALOGS DE CONFIRMACAO*/
	init_pair (C_SUCCESS, COLOR_BLACK, COLOR_GREEN);
	
	wattron (stdscr, COLOR_PAIR (6));
	mvwprintw (stdscr, 0, 1, "Userconf %s - Unix System User Configurator", VERSION);
	wattrset (stdscr, A_BOLD);
	mvwprintw (stdscr, T_LINES-1, 1, "ze-work's copyleft 04", VERSION);
	wattroff (stdscr, COLOR_PAIR (6));

	wbkgd (stdscr, COLOR_PAIR(1));
	wrefresh (stdscr);

	while (1) {
		open_mensagem (mensagem);
		wrefresh (stdscr);
		
		switch (opt) {
			case -1:
				opt = open_menu ();
			break;
			case 0:
				open_form ( form1 );
				opt = -1;
			break;
			case 1:
				open_form_list ( form_list );
				opt = -1;
			break;
			case 2:
				open_form_remove ( form_remove );
				opt = -1;
			break;
			case 3:
				kill(child, SIGKILL);
				quit ();
			break;
			default:
				opt = -1;
			break;
		}			
	}
		
	getch ();
	quit ();
	return 1;	
}
