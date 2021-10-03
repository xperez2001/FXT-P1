/***************************************************************************
 *            common.h
 *
 *  Fri Oct  1 16:04:36 2014
 *  Copyright  2014  mc
 *  <mcarmen@<host>>
 ****************************************************************************/

#include <unistd.h> //per tipus com el socklen_t
#include <netdb.h> //pel gethostbyname
#include <errno.h> //per gestionar errors
#include <sys/types.h> //per tipus com el uint
#include <netinet/in.h> //pel INADDR_ANY
#include <sys/socket.h> //per la creació de sockets

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFF_SIZE 512 

#define DEFAULT_PORT 2222

#define SWITCH_PORTS 6

#define MAC_SIZE 18 //17 + '\0'

#define MAX_MSG_SIZE 200

#define MAX_REPLY_SIZE 4

#define MSG_HELLO 1
#define MSG_HELLO_RP 2
#define MSG_LIST 3
#define MSG_LIST_RP 4
#define MSG_CONN_TO 5
#define MSG_CONN_ANY 6
#define MSG_CONN_ANY_RP 7
#define MSG_FREE 8
#define MSG_FREE_ALL 9
#define MSG_GEN_FILE 10
#define MSG_FINISH 11
#define MSG_OP_OK 12
#define MSG_OP_ERR 13

#define ERR_CODE_1 1
#define ERR_CODE_2 2
#define ERR_CODE_3 3
#define ERR_CODE_4 4
#define ERR_CODE_5 5

#define u_int16_t unsigned short

#define EMPTY_TXT_MSG "Empty port"

typedef  char swConnectionsMatrix[SWITCH_PORTS][MAC_SIZE];

typedef enum {
  FALSE = 0,
  TRUE = 1
} Bool;


// Macros per afegir i extreure el codi d'operacio d'un missatge
#define stshort(sval, addr) ( *((short *)(addr))=htons(sval) )
#define ldshort(addr) ( ntohs(*((short *)(addr)) ) ) 

/*
          2 bytes  12 bytes    
          ------------------------
HELLO_RP  | 02     | Hello World |
          ------------------------
*/
struct hello_rp_hdr{
	unsigned short opcode;
	char msg[12];
};

/*
            2 bytes  n bytes 
            ----------------------
            |        |18 bytes 
            |        |------------
LIST_RP     | 04     |[ MAC ] *6 |
            |        |------------
            ----------------------
*/
struct list_rp_hdr{
  unsigned short opcode;
  char connectionsMatrix[SWITCH_PORTS][MAC_SIZE];
};

/*
            2 bytes 2 bytes  18 bytes 
            ---------------------------
CONN_TO     | 05     | port  | MAC    |
            ---------------------------
*/
struct conn_to_hdr{
  unsigned short opcode;
  unsigned short port;
  char mac[MAC_SIZE];
};

/*
            2 bytes 18 bytes 
            ------------------
CONN_ANY    | 06     | MAC   |
            ------------------
*/
struct conn_any_hdr{
  unsigned short opcode;
  char mac[MAC_SIZE];
};

/*
            2 bytes  2 bytes 
            ------------------
CONN_ANY_RP | 07     | port  |
            ------------------
*/
struct conn_any_rp_hdr{
  unsigned short opcode;
  unsigned short port;
};

/*
            2 bytes  2 bytes 
            ------------------
FREE        | 08     | port  | 
            ------------------
*/
struct free_hdr{
  unsigned short opcode;
  unsigned short port;
};

/*
            2 bytes 2 bytes
            ---------------------
OP_ERR      | 13   | error_code |
            ---------------------
*/
struct op_err_hdr{
  unsigned short opcode;
  unsigned short error_code;
};

/** 
 * Sends a message formed by just the operation code .
 * @param sock socket used for the communication.
 * @param operation the operation code to be sent.
 */
void send_parameterless_msg(int sock, int operation);

/**
 * Stores the operation value in the first two bytes of the msg.
 * @param msg the message to be filled.
 * @param operation the operation code to be stored in the message.
 */
void build_parameterless_msg(char *msg, unsigned short operation);

/**
 * Stores the operation value in the first two bytes of the msg and the field1 
 * in the two following bytes.
 * @param msg the message to be filled.
 * @param operation the operation code to be stored in the message.
 * @param field1 the unsigned short to be stored after the operation code field.
 */
void build_two_unsigned_short_fields_msg(char * msg, unsigned short operation, 
  unsigned short field1);
  
  /**
 * @return TRUE if there is no mac address assigned to that port. 
 * In that case, there will be 18 zeros in the port position. FALSE otherwise.
 * @param port the switch port to check its availability.
 * @param swConnections the switch connections matrix
 */
Bool isPortAvailable(int port, swConnectionsMatrix swConnections);

/**
 * Prints all the MAC addresses bounded to a port or "empty" if there is no
 * connection.
 * @param out where to print the information, to the stdout or to a file.
 * @param swConnections The matrix with all the switch connections.
 */
void printConnections(FILE *out, swConnectionsMatrix swConnections);

