/***************************************************************************
 *            common.c
 *
 *  Fri Oct  1 16:04:36 2015
 *  Copyright  2014  mc
 *  <mcarmen@<host>>
 ****************************************************************************/
 
#include "common.h"

/** 
 * Sends a message stored in buffer containing an operation identifier 
 * through the socket.
 * @param sock socket used for the communication.
 * @param buffer to be sent.
 * @param operation the operation code to be sent.
 */
void send_parameterless_msg(int sock, int operation)
{ 
  char buffer[2];

  build_parameterless_msg(buffer, operation);
  //TODO send(...);
}

/**
 * Stores the operation value in the first two bytes of the msg.
 * @param msg the message to be filled.
 * @param operation the operation code to be stored in the message.
 */
void build_parameterless_msg(char *msg, unsigned short operation)
{
  //TODO stshort(...);
}

/**
 * Stores the operation value in the first two bytes of the msg and the field1 
 * in the two following bytes.
 * @param msg the message to be filled.
 * @param operation the operation code to be stored in the message.
 * @param field1 the unsigned short to be stored after the operation code field.
 */
void build_two_unsigned_short_fields_msg(char * msg, unsigned short operation, 
  unsigned short field1)
{
  //TODO stshort(...);
  //TODO stshort(...);
}

/**
 * @return TRUE if there is no mac address assigned to that port. 
 * In that case, there will be 18 zeros in the port position. FALSE otherwise.
 * @param port the switch port to check its availability.
 * @param swConnections the switch connections matrix
 */
Bool isPortAvailable(int port, swConnectionsMatrix swConnections)
{
  Bool isAvailable = FALSE;
  if (strlen(swConnections[port]) == 0) {
    isAvailable = TRUE;
  }
  
  return isAvailable;
}

/**
 * Prints all the MAC addresses bounded to a port or "empty" if there is no
 * connection.
 * @param out where to print the information, to the stdout or to a file.
 * @param swConnections The matrix with all the switch connections.
 */
void printConnections(FILE *out, swConnectionsMatrix swConnections)
{
  int i;
  char MAC_msg[MAC_SIZE];
  
  for(i=0; i< SWITCH_PORTS; i++)
  {
    if (isPortAvailable(i, swConnections) == FALSE){
      strcpy(MAC_msg,swConnections[i]);
    }
    else {
      strcpy(MAC_msg,EMPTY_TXT_MSG);      
    }
      
    fprintf(out, "IF%d -> %s\n", i, MAC_msg);
      
  }
  printf("\n");
}
