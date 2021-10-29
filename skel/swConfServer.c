/***************************************************************************
 *            swConfServer.c
 *
 *  Fri Mark  1 16:15:48 2013
 *  Copyright  2013  mc
 *  <mc@<host>>
 ****************************************************************************/

#include "swConfServer.h"



/**
 * @return  the port number from the command line or the default port if
 * no port has been specified in the command line. Returns -1 if the application
 * has been called with the wrong parameters.
 */
int getPort(int argc, char* argv[])
{
  int param;
  int port = DEFAULT_PORT;

  // We process the application execution parameters.
	while((param = getopt(argc, argv, "p:")) != -1){
		switch((char) param){
			case 'p':
			  // We modify the port variable just in case a port is passed as a
			  // parameter
				port = atoi(optarg);
				break;
			default:
				printf("Parametre %c desconegut\n\n", (char) param);
				port = -1;
		}
	}

	return port;
}



/**
 * Receives and process the request from a client.
 * @param the socket connected to the client.
 * @param switchConnections the structure whith all the switch connections
 * @return 1 if the user has exit the client application therefore the
 * connection whith the client has to be closed. 0 if the user is still
 * interacting with the client application.
 */
int process_msg(int sock, swConnectionsMatrix swConnections)
{
  unsigned short op_code;
  char buffer[MAX_BUFF_SIZE];
  int done = 0;

  if (recv(sock, &buffer, MAX_BUFF_SIZE, 0) == -1)
    printf("\tSERVER: recv -> Error\n");
  else
    printf("\tSERVER: recv -> Success\n");

  //TODO: obtenir el codi d'operació a partir del buffer:
  op_code = buffer[0];
  op_code *= 10;
  op_code |= buffer[1];

  printf("\tbuffer[0] = %d, buffer[1] = %d, opcode = %d \n", buffer[0], buffer[1], op_code);
 
  switch(op_code)
  {
    case MSG_HELLO:
      process_HELLO_msg(sock);
      break;
    case MSG_LIST:
      process_LIST_msg(sock, swConnections);
      break;
    case MSG_CONN_TO:
      process_CONN_TO_msg(sock, swConnections, (struct conn_to_hdr *)buffer);
      break;
    case MSG_CONN_ANY:
      process_CONN_ANY_msg(sock, swConnections, (struct conn_any_hdr *)buffer);
      break;
    case MSG_FREE:
      process_FREE_msg(sock, swConnections, (struct free_hdr *)buffer);
      break;
    case MSG_FREE_ALL:
      process_FREE_ALL_msg(sock, swConnections, buffer);
      break;
    case MSG_GEN_FILE:
      process_GEN_FILE_msg(sock, swConnections, buffer);
      break;
    case MSG_FINISH:
      close(sock);
      done = 1;
      break;
    default:
      perror("Message code does not exist.\n");
  }

  return done;
}

/**
 * Function that sends a HELLO_RP to the  client
 * @param sock the communications socket
 */
void process_HELLO_msg(int sock)
{
  struct hello_rp_hdr hello_rp;
  hello_rp.opcode = MSG_HELLO;
  char msg[12] = "Hello world";
  strcpy(hello_rp.msg, msg);

  if (send(sock, hello_rp.msg, sizeof(hello_rp.msg), 0) == -1)
    printf("\tSERVER: send -> Error\n");
  else
    printf("\tSERVER: send -> Succes\n");

/**
 * Function that sends the matrix whith the MAC addresses connected to each
 * port.
 * @param sock the communications socket.
 * @param swConnections the matrix with all the MAC addresses connected to each
 * port of the switch.
 */
}
void process_LIST_msg(int sock, swConnectionsMatrix swConnections)
{
  struct list_rp_hdr list_rp;

  memset(&list_rp, 0, sizeof(struct list_rp_hdr));

  //TODO: omplir el camp opcode amb MSG_LIST_RP
  //TODO: omplir el codi següent i descomentar:
  //TODO: memcpy(..., swConnections, MATRIX_SIZE);
  //TODO: send(.....);

  memcpy(list_rp.connectionsMatrix, swConnections, MATRIX_SIZE);
  list_rp.opcode = ntohs(MSG_LIST_RP);


  if (send(sock, &list_rp, sizeof(struct list_rp_hdr), 0) == -1)
    printf("\tSERVER: send -> Error\n");
  else
    printf("\tSERVER: send -> Success\n");

}

/**
 * Function that connects a MAC address to the given port if the port is
 * available.
 * If the port is available a Succes message is sent to the user.
 * If not the error is reported to the user.
 * @param sock the communications socket.
 * @param swConnections the matrix with all the MAC addresses connected to each
 * @param conn_to_msg Message received from the client already wrapped in the
 * corresponding structure.
 * port of the switch.
 */
void process_CONN_TO_msg(int sock, swConnectionsMatrix swConnections,
  struct conn_to_hdr * conn_to_msg)
{
  struct conn_to_hdr connect_to = *conn_to_msg;
  unsigned short port;
  char reply[MAX_REPLY_SIZE];
  int err_code;

  memset(reply, 0, MAX_REPLY_SIZE);

  //TODO: obtenir el port del missatge conn_to_msg:
  //port = ...(...);
  port = 0; // TODO: borrar aquesta linia
  if((err_code=checkPortAndMACFormat(port, connect_to.mac)) == 0)
  {
    if (isPortAvailable(port, swConnections))
    {
      //TODO: Assignar al port que ens han passat pel missatge
      // la mac que ens han passat també pel missatge:
      //strcpy(swConnections[port], .....);
      build_ok_msg(reply);
    }
    else
    {
      build_error_msg(reply, ERR_CODE_1);
    }
  }

  else
  {
    build_error_msg(reply, err_code);
  }

  //TODO: send(.....)
}


/**
 * Function that connects a MAC address to an available port if there is any
 * available port.
 * If the any port is available message is sent to the user informing about
 * the port being used.
 * If there is no port available the error is reported to the user.
 * @param sock the communications socket.
 * @param swConnections the matrix with all the MAC addresses connected to each
 * port of the switch.
 * @param conn_any_msg Message received from the client already wrapped in the
 * corresponding structure.
 */
void process_CONN_ANY_msg(int sock, swConnectionsMatrix swConnections,
  struct conn_any_hdr * conn_any_msg)
{
  struct conn_any_hdr connect_any = *conn_any_msg;
  struct conn_any_rp_hdr reply;
  char error_reply[MAX_REPLY_SIZE];
  unsigned short port;
  unsigned short err_code = -1;

  memset(error_reply, 0, sizeof(reply));

  if (isMACFormatRight(connect_any.mac) == TRUE)
  {
    port = getAvailablePort(swConnections);
    if (port != -1)
    {
      //TODO: Assignar al port que hem triat
      // la mac que ens han passat pel missatge:
      //strcpy(swConnections[port], .....);

      //TODO: omplir els camps de reply:
      //... = ...(MSG_CONN_ANY_RP);
      //... = ...(port);
    }
    else
    {
      err_code = ERR_CODE_2;
    }
  } //end isMACFormatRight
  else
  {
    err_code = ERR_CODE_5;
  }
  if (err_code == -1) //no error.
  {
    //TODO: send reply.
  }
  else
  {
    build_error_msg(error_reply, err_code);
    //TODO: send error_reply.
  }

}

/**
 * Function that releases the port specified by the user. If the port is already
 * empty an error message is sent to the client.
 * If not an OK message is sent to the client.
 * @param sock the communications socket.
 * @param swConnections the matrix with all the MAC addresses connected to each
 * port of the switch.
 * @param free_hdr Message received from the client already wrapped in the
 * corresponding structure.
 */
void process_FREE_msg(int sock, swConnectionsMatrix swConnections,
  struct free_hdr * buffer)
{
  struct free_hdr free_msg = *buffer;
  unsigned short port;
  char reply[MAX_REPLY_SIZE];

  memset(reply, 0, sizeof(reply));

  //TODO: obtenir el port del missatge free_msg:
  //port = ...(....);

  port = 8080; // TODO: borrar aquesta linia

  if(isPortInRang(port) == FALSE)
  {
    build_error_msg(reply, ERR_CODE_4);
  }
  else if(isPortAvailable(port, swConnections) == FALSE)
  {
    release_a_port(swConnections, port);
    build_ok_msg(reply);
  }
  else
  {
    build_error_msg(reply, ERR_CODE_3);
  }
  (void) free_msg;
  //TODO: send reply
}

/**
 * Function that releases all the ports of the switch.
 * After the operation an OK message is sent to the client. The OK message is
 * sent anyway although no port is being used.
 * @param sock the communications socket.
 * @param swConnections the matrix with all the MAC addresses connected to each
 * port of the switch.
 * @param free_hdr Message received from the client already wrapped in the
 * corresponding structure.
 */
void process_FREE_ALL_msg(int sock, swConnectionsMatrix swConnections,
  char * buffer)
{
  char reply[MAX_REPLY_SIZE];
  int i;

  memset(reply, 0, sizeof(reply));

  for(i = 0; i< SWITCH_PORTS; i++)
  {
    if(isPortAvailable(i, swConnections) == FALSE)
    {
      release_a_port(swConnections, i);
    }
  }
  build_ok_msg(reply);

  //TODO: send reply
}


/**
 * Function that generates a file with the switch connections information.
 * The file is generated in the server execution directory.
 * For each port, a row is generated indicating the port number the MAC address
 * associated or "Empty port" if the port is available:
 * IF0 -> Empty port
 * IF1 -> 00:00:00:00:00:00
 * ....
 * Once the file is generated a confirmation message is sent to the user.
 * @param sock the communications socket.
 * @param swConnections the matrix with all the MAC addresses connected to each
 * port of the switch.
 * @param free_hdr Message received from the client already wrapped in the
 * corresponding structure.
 * @return 0 if the file has been generated Successfuly, -1 otherwise.
 */
int process_GEN_FILE_msg(int sock, swConnectionsMatrix swConnections,
  char * buffer)
{
  FILE *fdesc;

	if( (fdesc = fopen(FILE_NAME, "w+")) == NULL){
		perror("No s'ha pogut obrir el fitxer correctament\n\n");
		return -1;
	}

	printConnections(fdesc, swConnections);

	fclose(fdesc);

  send_parameterless_msg(sock, MSG_OP_OK);

  return (0);
}


/**
 * @param port the port number to be checked.
 * @return true if the port is in the switch's rang of ports to be used.
 */
Bool isPortInRang(unsigned short port)
{
  Bool result = TRUE;

  if ((port >= SWITCH_PORTS) || (port <0)) {
    result = FALSE;
  }

  return result;
}


/**
 * @param port the MAC address to be checked.
 * @return true if the MAC address size is MAC_SIZE.
 */
Bool isMACFormatRight(char * mac)
{
  Bool result = TRUE;

  if(strlen(mac) != MAC_SIZE -1){
    result = FALSE;
  }

  return result;
}

/**
 * Checks if the the port is in the switch's rang of ports to be used and
 * if the MAC address size is MAC_SIZE.
 * @param port the port number to be checked.
 * @param port the MAC address to be checked.
 * @return 0 if both parameters are correct.
 * ERR_CODE_4 if the port is out of range.
 * ERR_CODE_5 if the MAC address size is wrong.
 * if both parameters are wront, the return error code is ERR_CODE_4
 */
int checkPortAndMACFormat(unsigned short port, char * mac)
{

  int result = 0;
  if(isPortInRang(port) == FALSE)
  {
    result =  ERR_CODE_4;
  }
  else if(isMACFormatRight(mac) == FALSE)
  {
     result =  ERR_CODE_5;
  }
  return result;
}

/**
 * Sets the MAC address in a port, to MAC_SIZE zeros.
 */
void release_a_port(swConnectionsMatrix swConnections, unsigned short port)
{
   memset(swConnections[port], 0, MAC_SIZE);
}

/**
 * Function that fills the msg parameter with an MSG_OP_OK code.
 * @param msg message to be filled with the OK code.
 */
void build_ok_msg(char *msg)
{
  build_parameterless_msg(msg, MSG_OP_OK);
}

/**
 * Function that fills the msg parameter with an MSG_OP_ERR code.
 * @param msg message to be filled with the NOK code.
 * @param error_code the error code to be set in the error message.
 */
void build_error_msg(char *msg, unsigned short error_code)
{
  build_two_unsigned_short_fields_msg(msg, MSG_OP_ERR, error_code);
}

/**
 * @return the first available port of the switch. A port is available if the
 * length of the MAC address is zero. -1 if all the ports are busy.
 * @param swConnections the switch connections matrix
 */
int getAvailablePort(swConnectionsMatrix swConnections)
{
  int i=0;
  int port = -1;
  Bool available = FALSE;

  while((available == FALSE) && (i < SWITCH_PORTS))
  {
    available = isPortAvailable(i, swConnections);
    i++;
  }

  if(available == TRUE) {
    port = i-1;
  }

  return port;
}



int main(int argc, char* argv[])
{
  printf("\nINICIANT SERVER...\n\n");

  swConnectionsMatrix swConnections;
  int con_sock;
  struct sockaddr_in client_addr, server_addr;
  socklen_t client_addrlen = sizeof(client_addr);
  int port = getPort(argc, argv);
  int done = 0;

  memset(swConnections, 0, MATRIX_SIZE);

  //TODO: setting up the socket for communication
  int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (s == -1)
	  printf("SERVER: socket -> Error\n");
  else 
    printf("SERVER: socket -> Succes\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, (struct sockaddr*)& server_addr, sizeof(server_addr)) == -1)
    printf("SERVER: bind -> Error\n");
  else
    printf("SERVER: bind -> Succes\n");

  if (listen(s, MAX_QUEUED_CON) == -1)
    printf("SERVER: listen -> Error\n");
  else
    printf("SERVER: listen -> Succes\n");

  while(1) {

    con_sock = accept(s, (struct sockaddr*)& server_addr, &client_addrlen);
    if (con_sock == -1)
      printf("SERVER: accept -> Error\n");
    else
      printf("SERVER: accept -> Succes\n"); 
    
   

    do {
    
      done = process_msg(con_sock, swConnections);
    }while(!done);

  }//end while



  
  return 0;
}

