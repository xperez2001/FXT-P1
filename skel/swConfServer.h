/***************************************************************************
 *            swConfServer.h
 *
 *  Fri Oct  1 16:04:36 2014
 *  Copyright  2014  mc
 *  <mcarmen@<host>>
 ****************************************************************************/



#include "common.h"

#define MAX_QUEUED_CON 10 // Max number of connections queued

#define MATRIX_SIZE SWITCH_PORTS*MAC_SIZE

#define FILE_NAME "swConf.txt"

/** 
 * Receives and process the request from a client.
 * @param the socket connected to the client.
 * @param switchConnections the structure whith all the switch connections
 * @return 1 if the user has exit the client application therefore the 
 * connection whith the client has to be closed. 0 if the user is still 
 * interacting with the client application.
 */
int process_msg(int sock, swConnectionsMatrix swConnections);

/**
 * Function that sends a HELLO_RP to the  client
 * @sock the communications socket
 */
void process_HELLO_msg(int sock);


/**
 * @return  the port number from the command line or the default port if 
 * no port has been specified in the command line. Returns -1 if the application
 * has been called with the wrong parameters.
 */
int getPort(int argc, char* argv[]);

/**
 * Function that sends the matrix whith the MAC addresses connected to each 
 * port. 
 * @param sock the communications socket.
 * @param swConnections the matrix with all the MAC addresses connected to each
 * port of the switch.
 */
void process_LIST_msg(int sock, swConnectionsMatrix swConnections);

/** 
 * @return the first available port of the switch. A port is available if the 
 * length of the MAC address is zero. -1 if all the ports are busy.
 * @param swConnections the switch connections matrix
 */
int getAvailablePort(swConnectionsMatrix swConnections);

/**
 * Function that connects a MAC address to the given port if the port is 
 * available. 
 * If the port is available a success message is sent to the user. 
 * If not the error is reported to the user. 
 * @param sock the communications socket.
 * @param swConnections the matrix with all the MAC addresses connected to each
 * @param conn_to_msg Message received from the client already wrapped in the
 * corresponding structure.
 * port of the switch.
 */
void process_CONN_TO_msg(int sock, swConnectionsMatrix swConnections, 
  struct conn_to_hdr * conn_to_msg);
  
  
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
  struct conn_any_hdr * conn_any_msg);
  
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
  struct free_hdr * buffer);
  
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
  char * buffer);
  
/**
 * Sets the MAC address in a port, to MAC_SIZE zeros.
 */ 
void release_a_port(swConnectionsMatrix swConnections, unsigned short port);    
  
  /**
 * Function that fills the msg parameter with an MSG_OP_OK code.
 * @param msg message to be filled with the OK code.
 */
void build_ok_msg(char *msg);

/**
 * Function that fills the msg parameter with an MSG_OP_ERR code.
 * @param msg message to be filled with the NOK code.
 * @param error_code the error code to be set in the error message.
 */
void build_error_msg(char *msg, unsigned short error_code);

/**
 * @param port the port number.
 * @return true if the port is in the switch's rang of ports to be used.
 */
Bool isPortInRang(unsigned short port);

/**
 * @param port the MAC address to be checked.
 * @return true if the MAC address size is MAC_SIZE.
 */
Bool isMACFormatRight(char * mac);
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
int checkPortAndMACFormat(unsigned short port, char * mac);

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
 * @return 0 if the file has been generated successfuly, -1 otherwise.
 */
int process_GEN_FILE_msg(int sock, swConnectionsMatrix swConnections, 
  char * buffer);


