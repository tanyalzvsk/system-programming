// Лазовскаяя Татьяна Андреевна








/*
* SPLPv1.c
* The file is part of practical task for System programming course.
* This file contains validation of SPLPv1 protocol.
*/




 /*
 ---------------------------------------------------------------------------------------------------------------------------
 # |      STATE      |         DESCRIPTION       |           ALLOWED MESSAGES            | NEW STATE | EXAMPLE
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 1 | INIT            | initial state             | A->B     CONNECT                      |     2     |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 2 | CONNECTING      | client is waiting for con-| A<-B     CONNECT_OK                   |     3     |
 |                 | nection approval from srv |                                       |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 3 | CONNECTED       | Connection is established | A->B     GET_VER                      |     4     |
 |                 |                           |        -------------------------------+-----------+----------------------
 |                 |                           |          One of the following:        |     5     |
 |                 |                           |          - GET_DATA                   |           |
 |                 |                           |          - GET_FILE                   |           |
 |                 |                           |          - GET_COMMAND                |           |
 |                 |                           |        -------------------------------+-----------+----------------------
 |                 |                           |          GET_B64                      |     6     |
 |                 |                           |        ------------------------------------------------------------------
 |                 |                           |          DISCONNECT                   |     7     |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 4 | WAITING_VER     | Client is waiting for     | A<-B     VERSION ver                  |     3     | VERSION 2
 |                 | server to provide version |          Where ver is an integer (>0) |           |
 |                 | information               |          value. Only a single space   |           |
 |                 |                           |          is allowed in the message    |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 5 | WAITING_DATA    | Client is waiting for a   | A<-B     CMD symbsCommand CMD                 |     3     | GET_DATA a GET_DATA
 |                 | response from server      |                                       |           |
 |                 |                           |          CMD - command sent by the    |           |
 |                 |                           |           client in previous message  |           |
 |                 |                           |          symbsCommand - string which contains |           |
 |                 |                           |           the following allowed cha-  |           |
 |                 |                           |           racters: small latin letter,|           |
 |                 |                           |           digits and '.'              |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 6 | WAITING_B64_DATA| Client is waiting for a   | A<-B     B64: symbsCommand                    |     3     | B64: SGVsbG8=
 |                 | response from server.     |          where symbsCommand is a symbsBase64 string|           |
 |                 |                           |          only 1 space is allowed      |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 7 | DISCONNECTING   | Client is waiting for     | A<-B     DISCONNECT_OK                |     1     |
 |                 | server to close the       |                                       |           |
 |                 | connection                |                                       |           |
 ---------------------------------------------------------------------------------------------------------------------------

 IN CASE OF INVALID MESSAGE THE STATE SHOULD BE RESET TO 1 (INIT)

 */

#include "splpv1.h"
#include "string.h"

 /* FUNCTION   :  validate_message
 *
 * PURPOSE:
 *    This function is called for each SPLPv1 message between client
 *    and server
 *
 * PARAMETERS:
 *    msg - pointer to a structure which stores information about
 *    message
 *
 * RETURN VALUE:
 *    MESSAGE_VALID if the message is correct
 *    MESSAGE_INVALID if the message is incorrect or out of protocol
 *    state
 */




const char symbsBase64[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const char symbsCommand[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int state = 1;
int cmd;

enum test_status validate_message(struct Message* msg) {
    char* msgTxt = msg->text_message;
    if (msg->direction == A_TO_B) {
        if (state == 1) {                  
            if (0 == strcmp(msgTxt, "CONNECT")) {
                state = 2;
                return MESSAGE_VALID;
            }
        }
        else if (state == 3) {
            if (0 == strcmp(msgTxt, "GET_VER")) {
                state = 4;
                return MESSAGE_VALID;
            }
            else if (0 == strcmp(msgTxt, "GET_DATA")) {
                cmd = 0;
                state = 5;
                return MESSAGE_VALID;
            }
            else if (0 == strcmp(msgTxt, "GET_FILE")) {
                cmd = 1;
                state = 5;
                return MESSAGE_VALID;
            }
            else if (0 == strcmp(msgTxt, "GET_COMMAND")) {
                cmd = 2;
                state = 5;
                return MESSAGE_VALID;
            }
            else if (0 == strcmp(msgTxt, "GET_B64")) {
                state = 6;
                return MESSAGE_VALID;
            }
            else if (0 == strcmp(msgTxt, "DISCONNECT")) {
                state = 7;
                return MESSAGE_VALID;
            }
        }
    }

    else if (msg->direction == B_TO_A) {
        if (state == 2) {
            if (0 == strcmp(msgTxt, "CONNECT_OK")) {
                state = 3;
                return MESSAGE_VALID;
            }
        }
        else if (state == 7) {
            if (0 == strcmp(msgTxt, "DISCONNECT_OK")) {
                state = 1;
                return MESSAGE_VALID;
            }
        }



        else if (state == 4) {
            if (!strncmp(msgTxt, "VERSION ", 8)) {
                msgTxt += 8;                             
                for (msgTxt; *msgTxt != '\0'; msgTxt++)   
                    if (*msgTxt < '0' || *msgTxt > '9') {
                        state = 1;
                        return MESSAGE_INVALID;
                    }
                state = 3;
                return MESSAGE_VALID;
                    
            }
        }

        else if (state == 5) {
            if (cmd == 0 && !strncmp(msgTxt, "GET_DATA ", 9)) 
            {
                msgTxt += 9; 

                while (symbsCommand[*msgTxt])
                {
                    msgTxt++; 
                }

                if (*msgTxt == ' ') { 
                    msgTxt ++; 
                    if (0 == strcmp(msgTxt, "GET_DATA")) { 
                        state = 3;
                        return MESSAGE_VALID;
                    }
                }
            }
            if (cmd == 1 && !strncmp(msgTxt, "GET_FILE ", 9)) 
            {
                msgTxt += 9;

                while (symbsCommand[*msgTxt])
                {
                    msgTxt++;
                }

                if (*msgTxt == ' ') { 
                    msgTxt++; 
                    if (0 == strcmp(msgTxt , "GET_FILE")) { 
                        state = 3;
                        return MESSAGE_VALID;
                    }
                }
            }
            if (cmd == 2 && !strncmp(msgTxt, "GET_COMMAND ", 12)) 
            {
                msgTxt += 12; 

                while (symbsCommand[*msgTxt])
                {
                    msgTxt++; 
                }

                if (*msgTxt == ' ') { 
                    msgTxt++; 
                    if (0 == strcmp(msgTxt, "GET_COMMAND")) { 
                        state = 3;
                        return MESSAGE_VALID;
                    }
                }
            }
        }


        if (state == 6)
        {
            if (strncmp(msgTxt, "B64: ", 5) == 0)
            {
                msgTxt += 5;
                char* msgTxt_length = msgTxt;                      
                while (symbsBase64[*msgTxt_length])
                {
                    msgTxt_length++;
                }
               

                if ((msgTxt_length - msgTxt) % 4 == 0 && msgTxt_length[0] == '\0') 
                {
                    state = 3;
                    return MESSAGE_VALID;
                }
                if ((msgTxt_length - msgTxt ) % 4 == 3 && msgTxt_length[1] == '\0') 
                {
                    state = 3;
                    return MESSAGE_VALID;
                }
                if (((msgTxt_length - msgTxt ) % 4 == 2 && msgTxt_length[2] == '\0'))  
                {
                    state = 3;
                    return MESSAGE_VALID;
                }
            }
        }
    }
    state = 1;
    return MESSAGE_INVALID;
}
