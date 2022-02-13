// 非推奨な機能を使ってもエラーにならないようにするお行儀の悪いマクロ
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// ヘッダ
#include <stdio.h>
#include <WinSock2.h>

// 関数プロトタイプ宣言
void printErrorWSAStartup(int err);

// エントリーポイント
int main() 
{
	WSADATA wsaData;

	struct sockaddr_in server_conf;
	SOCKET server;
	char response[2048];

	// winsock2の初期化
	int err = WSAStartup(MAKEWORD(2, 0), &wsaData);

	if (err != 0) {
		printErrorWSAStartup(err);
		return 1;
	}

	// ソケットの作成
	server = socket(AF_INET, SOCK_STREAM, 0);

	if (server == INVALID_SOCKET) {
		printf("error : %d\n", WSAGetLastError());
		return 1;
	}

	// 接続先指定用構造体の準備
	server_conf.sin_family = AF_INET;
	server_conf.sin_port = htons(12345);
	server_conf.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// サーバーに接続
	connect(server, (struct sockaddr*)&server_conf, sizeof(server_conf));
	send(server, "Hello", 6, 0);

	// サーバーからデータを受信
	memset(response, 0, sizeof(response));
	int n = recv(server, response, sizeof(response), 0);
	printf("%d, %s\n", n, response);

	// winsock2の終了
	WSACleanup();

	return 0;
}

/*
* WSAStartupのエラーの内容を表示する
* int err: エラーコード
*/
void printErrorWSAStartup(int err)
{
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