#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

// 用語
// WSA = Windows Sockets API
int main()
{
	// Windows Sockets APIのデータを格納する変数を用意する
	WSADATA wsaData;
	SOCKET sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;
	SOCKET sock;
	int n;
	char buf[2048];
	char inbuf[2048];

	// WSAStartupはソケットプログラムを開始するための準備をするもので、一度だけ呼ぶ必要がある。
	// 第一引数には16bitの変数にAPIのバージョンを指定して渡す。
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
		return 1;
	}

	// ソケットの作成
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	if (sock0 == INVALID_SOCKET) {
		printf("error : %d\n", WSAGetLastError());
		return 1;
	}

	// ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(sock0, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		printf("error : %d\n", WSAGetLastError());
		return 1;
	}

	// TCPクライアントからの接続要求を待てる状態にする
	if (listen(sock0, 5) != 0) {
		printf("error : %d\n", WSAGetLastError());
		return 1;
	}

	// レスポンスメッセージを作成
	memset(buf, 0, sizeof(buf));
	_snprintf(
		buf, sizeof(buf),
		"HTTP/1.1 200 OK \r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"HELLO\r\n"
	);

	// TCPクライアントからの接続要求を受け付ける
	while (1) {
		len = sizeof(client);
		sock = accept(sock0, (struct sockaddr*)&client, &len);

		if (sock == INVALID_SOCKET) {
			printf("error : %d\n", WSAGetLastError());
			break;
		}

		printf("accepted connection:\n");
		printf("from=%s\n", inet_ntoa(client.sin_addr));
		printf("port=%d\n", ntohs(client.sin_port));

		memset(inbuf, 0, sizeof(inbuf));
		recv(sock, inbuf, sizeof(inbuf), 0);
		printf("%s", inbuf);

		// データ送信
		n = send(sock, buf, (int)strlen(buf), 0);

		if (n < 1) {
			printf("error : %d\n", WSAGetLastError());
			break;
		}

		closesocket(sock);
	}

	WSACleanup();

	return 0;
}