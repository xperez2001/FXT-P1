/***************************************************************************
 *            swConfClient.h
 *
 *  Fri Oct  1 16:04:36 2014
 *  Copyright  2021  mc
 *  <mcarmen@<host>>
 ****************************************************************************/
#include "swConfClient.h"

/**
 * Function that sets the field addr->sin_addr.s_addr from a host name
 * address.
 * @param addr struct where to set the address.
 * @param host the host name to be converted
 * @return -1 if there has been a problem during the conversion process.
 */
int setaddrbyname(struct sockaddr_in *addr, char *host)
{
  struct addrinfo hints, *res;
	int status;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(host, NULL, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return -1;
  }

  addr->sin_addr.s_addr = ((struct sockaddr_in *)res->ai_addr)->sin_addr.s_addr;

  freeaddrinfo(res);

  return 0;
}

/**
 * Shows the menu options.
 */
void printa_menu()
{
		// Mostrem un menu perque l'usuari pugui triar quina opcio fer

		printf("\nAplicatiu per l'automatrícula\n");
		printf("  0. Hello\n");
		printf("  1. Connectar a un port\n");
		printf("  2. Connectar a qualsevol port\n");
		printf("  3. Alliberar la connexió d'un port\n");
		printf("  4. Alliberar totes les connexions.\n");
		printf("  5. Generar un fitxer amb la configuració actual.\n");
		printf("  6. Sortir\n\n");
		printf("Escolliu una opcio: ");
}


/**
 * and closes the socket connected to the server.
 * @param sock socket used for the communication.
 */
void process_finish_operation(int sock)
{
  //TODO: send_parameterless_msg(...);
  //TODO close(...);
  exit(0);
}


/**
 * Sends a HELLO message and prints the server response.
 * @param sock socket used for the communication.
 */
void process_hello_operation(int sock)
{
  struct hello_rp_hdr hello_rp;

  memset(&hello_rp, '\0', sizeof(hello_rp));

  send_parameterless_msg(sock, hello_rp.opcode);
  recv(sock, &hello_rp.msg, sizeof(hello_rp.msg), MSG_WAITALL);
  printf(hello_rp.msg);
}

/**
 * Function that sends a connect_to message and waits for a reply.
 * Asks for the port and the mac address to connect to.
 * @param sock socket used for the communication.
 */
void process_connect_to_operation(int sock)
{
  struct conn_to_hdr connect_to;
  char reply[MAX_REPLY_SIZE];

  //NOTA: L'operation code és  MSG_CONN_TO
  //TODO  connect_to.opcode = htons(...);
  //TODO omplir port
  set_MAC_from_stdin(connect_to.mac);

  //TODO send(.....);
  //TODO recv(..., reply, ....);
  show_boolean_operation_feedback(reply);
}

/**
 * Function that sends a connect_to_any message and waits for a reply.
 * Asks for the mac address to connect to any.
 * @param sock socket used for the communication.
 */
void process_connect_to_any_operation(int sock)
{
  struct conn_any_hdr connect_to_any;
  struct conn_any_rp_hdr reply;
  (void) reply;
  //TODO omplir el opcode MSG_CONN_ANY
  set_MAC_from_stdin(connect_to_any.mac);

  //TODO send(....)
  //TODO recv(.., reply,..)

  //TODO obtenir el codi d'operació tot omplint el
  //puntejat i descomentar aquest codi
  /*
  if(.... == MSG_CONN_ANY_RP)
  {
    //mostrar el port assignat tot omplint el puntejat
    printf("%s %hu\n\n",CONN_ANY_TXT_MSG, .....);
  }
  else
  {
    show_boolean_operation_feedback((char *)&reply);
  }
  */
}

/**
 * Function that sends a free message and waits for a reply.
 * Asks for the port to be released.
 * @param sock socket used for the communication.
 */
void process_free_operation(int sock)
{
  struct free_hdr free_msg;
  char reply[MAX_REPLY_SIZE];
  (void) free_msg;
  //TODO omplir opcode
  //TODO omplir port ........=....(get_port_from_stdin());

  //TODO send(....)
  //TODO recv(..., reply, ...);
  show_boolean_operation_feedback(reply);

}

/**
 * Function that sends a free_all message and waits for a reply.
 * @param sock socket used for the communication.
 */
void process_free_all_operation(int sock)
{
  process_parameterless_boolean_operation_with_feedback(sock, MSG_FREE_ALL);
}


/**
 * Function that sends a free_all message and waits for a reply.
 * @param sock socket used for the communication.
 */
void process_gen_file_operation(int sock)
{
  process_parameterless_boolean_operation_with_feedback(sock, MSG_GEN_FILE);
}

/**
 * Generic function that sends a message with just the operation field, waits
 * for a boolean reply, and prints it.
 * @param sock socket used for the communication.
 * @param msg The operation code to be sent.
 */
void process_parameterless_boolean_operation_with_feedback(int sock,
  unsigned short msg)
{
  char reply[MAX_REPLY_SIZE];


  send_parameterless_msg(sock, msg);
  //TODO recv(..., reply, ....);
  show_boolean_operation_feedback(reply);
}

/**
 * Function that requests to the user for the MAC address and gets the
 * stdin value.
 * @param a ptr where to store the mac address got from the stdin.
 */
void set_MAC_from_stdin(char * mac)
{
  printf("Indica l'adreça MAC a connectar:\n");
  scanf("%s", mac);
  printf("\n");
}

/**
 * Function that prints the result of the operation, which can be success or
 * failure. In a failure case the error message is shown.
 * @param reply the message received by the server.
 */
void show_boolean_operation_feedback(char *reply)
{
  int error_code;
  char err_msg[MAX_ERR_MSG_SIZE];
  (void) err_msg;
  (void) error_code;
  //TODO obtenir, a partir de reply, el codi d'operació.
  // tot omplint el puntejat i descomentant el codi.
  /*
  if(ldshort(..... == MSG_OP_OK)
  {
    printf("%s\n\n", OK_MSG);
  }
  else
  {
    error_code = .....;//obtenir el codi d'error del missatge
    get_error_msg(error_code, err_msg);
    printf("%s: %s\n\n", NOK_MSG, err_msg);
  }
  */
}

/**
 * Function that deppendig on the error code, sets the err_msg parameter
 * to the proper error message.
 * @param err_code the error code.
 * @param err_msg the error message to be built.
 */
void get_error_msg(int err_code, char *err_msg)
{

    switch(err_code)
    {
      case (ERR_CODE_1):
        strcpy(err_msg, ERR_MSG_1);
        break;
      case (ERR_CODE_2):
        strcpy(err_msg, ERR_MSG_2);
        break;
      case (ERR_CODE_3):
        strcpy(err_msg, ERR_MSG_3);
        break;
      case (ERR_CODE_4):
        strcpy(err_msg, ERR_MSG_4);
        break;
      case (ERR_CODE_5):
        strcpy(err_msg, ERR_MSG_5);
        break;
      default:
        strcpy(err_msg, ERR_MSG_1);

    }
}


/**
 * Function that requests to the user for the switch port where connect a MAC
 * address and gets the stdin value.
 * @return the port got from the stdin.
 */
unsigned short get_port_from_stdin()
{
  unsigned short port;

  printf("Indica el port: \n");
  scanf("%hu", &port);
  printf("\n");

  return port;
}

/**
 * Sends a LIST message and prints the server response: all the MAC addresses
 * connected to each one of the switch ports.
 * @param sock socket used for the communication.
 */
void do_list_operation(int sock)
{
  struct list_rp_hdr list_rp;
  (void) list_rp;
  //TODO send_parameterless_msg(..., MSG_LIST);
  //TODO recv(.........);

  //TODO comprovar el codi d'operació tot omplint els
  //puntejats i descomentar el codi.
  /*
  if (ntohs(list_rp.opcode) == MSG_LIST_RP)
  {
    printConnections(stdout, list_rp.connectionsMatrix);
  }
  else
  {
    printf("Message out of the sequence");
  }
  */
}

/**
 * Function that process the menu option set by the user by calling
 * the function related to the menu option.
 * @param s The communications socket
 * @param option the menu option specified by the user.
 */
void process_menu_option(int s, int option)
{
  switch(option){
    // Opció HELLO
    case OP_HELLO:
      process_hello_operation(s);
      break;
    case OP_CONNECT_TO:
      process_connect_to_operation(s);
      break;
    case OP_CONNECT_ANY:
      process_connect_to_any_operation(s);
      break;
    case OP_FREE:
      process_free_operation(s);
      break;
    case OP_FREE_ALL:
      process_free_all_operation(s);
      break;
    case OP_GEN_FILE:
      process_gen_file_operation(s);
      break;
    case OP_FINISH:
      process_finish_operation(s);
      break;

    default:
          printf("Invalid menu option\n");
  		}
}

int main(int argc, char *argv[])
{
	int param; // to process the params we use to call the application
	int ctrl_host = 0;
	int port = DEFAULT_PORT;
	char *host; // to get the host name from the optarg parameter
	int option = 0;

  //TODO declarar les variables per crear els sockets.
  int s;
  struct sockaddr_in server_addr;

	// We process the application execution parameters.
	while((param = getopt(argc, argv, "p:h:")) != -1){
		switch((char) param){
			case 'p':
			  // We modify the port variable just in case a port is passed as a
			  // parameter
				port = atoi(optarg);
				break;
			case 'h':
				// Capturem el nom del host on es troba el servidor (ex: ccd-dc10.uab.es)
				// Creem dinamicament espai per emmagatzemar la cadena de text que conte el host
				// en la variable host. L'espai requerit es la longitud de la cadena (obtinguda
				// amb strlen, mes 1, donat que totes les cadenes de text acabem amb el caracter '\0'
				// que no es tingut en compte per strlen, ni es mostra per pantalla al fer printf

				host = (char*) malloc(sizeof(char)*strlen(optarg)+1);
				// Un cop creat l'espai, podem copiar la cadena
				strcpy(host, optarg);
				ctrl_host = 1;
				break;
			default:
				printf("Parametre %c desconegut\n\n", (char) param);
				return -1;
		}
	}

	// Comprovem que s'hagi introduit un host. En cas contrari, terminem l'execucio de
	// l'aplicatiu
	if(!ctrl_host){
		perror("No s'ha especificat el nom del servidor\n\n");
		return -1;
	}

  //TODO: setting up the socket for communication
  s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  setaddrbyname(&server_addr, host);

  connect(s, (struct sockaddr*)& server_addr, sizeof(server_addr));
  process_hello_operation(s);
  do{
      do_list_operation(s);
      printa_menu();
		  // getting the user input.
		  scanf("%d",&option);
		  printf("\n\n");
		  process_menu_option(s, option);

	  }while(option != OP_FINISH); //end while(opcio)

  return 0;
  //
}
