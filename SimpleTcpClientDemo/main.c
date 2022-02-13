#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

// AF_INET: AFはAddress Familyの略だろう、INETはIPv4で通信するという事を指定する。
// sin_familyなどのsinはsockaddr_inを略したsin、またsockaddr_inはsocket addreess, internetの略
int main() {
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[2048];

	// winsock2の初期化
	int err = WSAStartup(MAKEWORD(2, 0), &wsaData);

	if (err != 0) {
		switch (err) {
		case WSASYSNOTREADY: // ネットワークサブシステムがネットワークへの接続を準備できてない
			printf("WSASYSNOTREADY\n");
			break;
		case WSAVERNOTSUPPORTED: // winsockのバージョンがサポートされていない
			printf("WSAVERNOTSUPPORTED\n");
			break;
		case WSAEINPROGRESS:
			printf("WSAEINPROGRESS\n");
			break;
		case WSAEPROCLIM:
			printf("WSAEPROCLIM\n");
			break;
		case WSAEFAULT:
			printf("WSAEFAULT\n");
			break;
		}
	}

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET) {
		printf("error : %d\n", WSAGetLastError());
		return 1;
	}

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// サーバーに接続
	connect(sock, (struct sockaddr*)&server, sizeof(server));

	send(sock, "Hello", 5, 0);

	// サーバーからデータを受信
	memset(buf, 0, sizeof(buf));
	int n = recv(sock, buf, sizeof(buf), 0);
	printf("%d, %s\n", n, buf);

	// winsock2の終了
	WSACleanup();

	return 0;
}