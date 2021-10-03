
/***************************************************************************
 *            swConfClient.h
 *
 *  Fri Oct  1 16:04:36 2014
 *  Copyright  2021  mc
 *  <mcarmen@<host>>
 ****************************************************************************/

#include "common.h"

#define OP_HELLO 0
#define OP_CONNECT_TO 1
#define OP_CONNECT_ANY 2
#define OP_FREE 3
#define OP_FREE_ALL 4
#define OP_GEN_FILE 5
#define OP_FINISH 6

#define OK_MSG "Operació finalitzada amb èxit"
#define NOK_MSG "L'operació no s'ha pogut dur a terme degut a: "

#define MAX_ERR_MSG_SIZE 100

#define ERR_MSG_1 "El port ja està ocupat"
#define ERR_MSG_2 "No hi han ports lliures"
#define ERR_MSG_3 "El port està lliure"
#define ERR_MSG_4 "El port està fora de rang"
#define ERR_MSG_5 "El format de l'adreça MAC és incorrecte"

#define CONN_ANY_TXT_MSG "L'equip s'ha connectat al port: "



/**
 * Function that sets the field addr->sin_addr.s_addr from a host name 
 * address.
 * @param addr struct where to set the address.
 * @param host the host name to be converted
 * @return -1 if there has been a problem during the conversion process.
 */
int setaddrbyname(struct sockaddr_in *addr, char *host);

/**
 * Shows the menu options. 
 */
void printa_menu();


/**
 * and sends the finish message and prints the server response.
 * @param sock socket used for the communication.
 */
void process_finish_operation(int sock);



/**
 * Generic function that sends a message with just the operation field, waits
 * for a boolean reply, and prints it.
 * @param sock socket used for the communication.
 * @param msg The operation code to be sent.
 */
void process_parameterless_boolean_operation_with_feedback(int sock, 
  unsigned short msg);
  
/**
 * Function that sends a free_all message and waits for a reply.
 * @param sock socket used for the communication.
 */
void process_gen_file_operation(int sock);  

/**
 * Function that sends a free_all message and waits for a reply.
 * @param sock socket used for the communication.
 */
void process_free_all_operation(int sock);

/**
 * Closes the socket connected to the server.
 * @param sock socket used for the communication.
 */
void process_finish_operation(int sock);

/**
 * Sends a HELLO message and prints the server response.
 * @param sock socket used for the communication.
 */
void process_hello_operation(int sock);

/**
 * Function that sends a connect_to message and waits for a reply.
 * Asks for the port and the mac address to connect to.
 * @param sock socket used for the communication.
 */
void process_connect_to_operation(int sock);

/**
 * Function that sends a connect_to_any message and waits for a reply.
 * Asks for the mac address to connect to any.
 * @param sock socket used for the communication.
 */
void process_connect_to_any_operation(int sock);

/**
 * Function that sends a free message and waits for a reply.
 * Asks for the port to be released.
 * @param sock socket used for the communication.
 */
void process_free_operation(int sock);

/**
 * Function that requests to the user for the MAC address and gets the 
 * stdin value. 
 * @param a ptr where to store the mac address got from the stdin.
 */
void set_MAC_from_stdin(char * mac);

/**
 * Function that requests to the user for the switch port where connect a MAC 
 * address and gets the stdin value.
 * @return the port got from the stdin.
 */
unsigned short get_port_from_stdin();

/**
 * Sends a LIST message and prints the server response: all the MAC addresses
 * connected to each one of the switch ports.
 * @param sock socket used for the communication.
 */
void do_list_operation(int sock);

/** 
 * Function that process the menu option set by the user by calling 
 * the function related to the menu option.
 * @param sock The communications socket
 * @param option the menu option specified by the user.
 */
void process_menu_option(int sock, int option);

/**
 * Function that deppendig on the error code, sets the err_msg parameter 
 * to the proper error message.
 * @param err_code the error code.
 * @param err_msg the error message to be built.
 */
void get_error_msg(int err_code, char *err_msg);

/**
 * Function that prints the result of the operation, which can be success or 
 * failure. In a failure case the error message is shown.
 * @param reply the message received by the server.
 */
void show_boolean_operation_feedback(char *reply);


