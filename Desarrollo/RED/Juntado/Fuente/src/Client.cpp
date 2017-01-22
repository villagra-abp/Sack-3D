#include "Client.h"
#include "PhysicWorld.h"

Client::Client()
{
    //ctor
    //unsigned char GetPacketIdentifier(RakNet::Packet *p);
    client=RakNet::RakPeerInterface::GetInstance();
    clientID=RakNet::UNASSIGNED_SYSTEM_ADDRESS;
    isServer=false;
}

void Client::iniciar(){
    RakNet::SocketDescriptor socketDescriptor(atoi(PhysicWorld::Instance()->getPlayer()->getClientPort()),0);
	socketDescriptor.socketFamily=AF_INET;
	client->Startup(8,&socketDescriptor, 1);
	client->SetOccasionalPing(true);

	#if LIBCAT_SECURITY==1
        char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
        FILE *fp = fopen("publicKey.dat","rb");
        fread(public_key,sizeof(public_key),1,fp);
        fclose(fp);
    #endif

    #if LIBCAT_SECURITY==1
        RakNet::PublicKey pk;
        pk.remoteServerPublicKey=public_key;
        pk.publicKeyMode=RakNet::PKM_USE_KNOWN_PUBLIC_KEY;
        bool b = client->Connect(PhysicWorld::Instance()->getPlayer()->getIp(), atoi(PhysicWorld::Instance()->getPlayer()->getServerPort()), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"), &pk)==RakNet::CONNECTION_ATTEMPT_STARTED;
    #else
	RakNet::ConnectionAttemptResult car = client->Connect(PhysicWorld::Instance()->getPlayer()->getIp(), atoi(PhysicWorld::Instance()->getPlayer()->getServerPort()), "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"));
	RakAssert(car==RakNet::CONNECTION_ATTEMPT_STARTED);
    #endif

	printf("\nMy IP addresses:\n");
	unsigned int i;
	for (i=0; i < client->GetNumberOfAddresses(); i++)
	{
		printf("%i. %s\n", i+1, client->GetLocalIP(i));
	}

	printf("My GUID is %s\n", client->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());

}

void Client::enviar(){

    char aux[30];
    char id[30];
    char estado[30];
    char posx[30];
    char posy[30];
    bool muerto;

    vector3df posicion = PhysicWorld::Instance()->getPlayer()->getPosition();
    muerto = PhysicWorld::Instance()->getPlayer()->getFingirMuerte();

    //std::cout<<posicion.X<<std::endl;
    float auxiliarx;
    float auxiliary;
    auxiliarx = posicion.X * 10000;
    auxiliary = posicion.Y * 10000;

    if(muerto) strncpy(estado, "0", sizeof(estado));
    else strncpy(estado, "1", sizeof(estado));

    sprintf(posx, "%.0f", auxiliarx);
    sprintf(posy, "%.0f", auxiliary);
/*
    #ifdef _WIN32
		Sleep(30);
    #else
        usleep(30 * 1000);
    #endif
*/
    strncpy(id, "0", sizeof(id));

    char posx2[30];
    strncpy(posx2, posx, sizeof(posx2));
    char posy2[30];
    strncpy(posy2, posy, sizeof(posy2));

    strncat (id, " ", 6);
    strncat (id, estado, 6);
    strncat (id, " ", 6);
    //strncat (id, "x", 6);
    strncat (id, posx2, 6);
    strncat (id, " ", 6);
    //strncat (id, "y", 6);
    strncat (id, posy2, 6);
    strncpy(aux, id, sizeof(aux));

    client->Send(aux, (int) strlen(aux)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}


void Client::recibir(){

    for (p=client->Receive(); p; client->DeallocatePacket(p), p=client->Receive())
		{
			// We got a packet, get the identifier with our handy function
			packetIdentifier = GetPacketIdentifier(p);

			// Check if this is a network message packet
			switch (packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED\n");
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_CONNECTION_LOST\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
				printf("My external address is %s\n", client->GetExternalID(p->systemAddress).ToString(true));
				break;
			default:
				// It's a client, so just show the message
				//printf("%s\n", p->data);
				break;
			}

			/////////TRABAJO DEL MENSAJE\\\\\\\\\\\\

			char recibido[30];
			char id_r[30];
			char estado_r[30];
			char posx_r[30];
			char posy_r[30];
			int iterador=0;
			long int x;
			long int y;
			strncpy(recibido, reinterpret_cast<const char*>(p->data), sizeof(recibido));
			//std::cout<<"ESTOY TRATANDO EL MENSAJE->"<<recibido<<std::endl;
			char * msg;
			msg = strtok(recibido, " ");
			while(msg != NULL){
                switch (iterador)
                {
                    case 0:
                        strncpy(id_r, msg, sizeof(id_r));
                        break;
                    case 1:
                        strncpy(estado_r, msg, sizeof(estado_r));
                        break;
                    case 2:
                        strncpy(posx_r, msg, sizeof(posx_r));
                        break;
                    case 3:
                        strncpy(posy_r, msg, sizeof(posy_r));
                        break;
                }
                msg = strtok(NULL, " ");
                iterador++;
            }

            //std::cout<<"ESTOY TRATANDO EL MENSAJE->"<<id_r<<"->"<<estado_r<<"->"<<posx_r<<"->"<<posy_r<<std::endl;
           /*sscanf(posx_r, "%d", x);
           sscanf(posy_r, "%d", y);*/
            x = atol(posx_r);
            y = atol(posy_r);

            PhysicWorld::Instance()->getPlayerRed()->setx(x);
            PhysicWorld::Instance()->getPlayerRed()->sety(y);
    }
}

unsigned char Client::GetPacketIdentifier(RakNet::Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char) p->data[0];
}

Client::~Client()
{
    //dtor
}
