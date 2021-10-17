#include <iostream>
#include <enet/enet.h>

int main(int argc, char** argv) {

	size_t PlayerLimit = 32;


	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENet!\n");
		return EXIT_FAILURE;
	}

	atexit(enet_deinitialize);
	
	ENetAddress address;
	ENetEvent event;
	ENetHost* server;

	address.host = ENET_HOST_ANY;
	address.port = 1234;

	server = enet_host_create(&address, PlayerLimit, 1, 0, 0);

	if (server == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet server host!\n");
		return EXIT_FAILURE;
	}

	// GAME LOOP

	while (true) {
		while (enet_host_service(server, &event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->address.host,
					event.peer->address.port,
					event.channelID);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%x:%u disconnected.\n", event.peer->address.host, event.peer->address.port);
				break;

			}
		}
	}
	enet_host_destroy(server);

	return EXIT_SUCCESS;
	// GAME LOOP END
}