// 非推奨な機能を使ってもエラーにならないようにするお行儀の悪いマクロ
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// ヘッダ
#include <stdio.h>
#include <WinSock2.h>

// 関数プロトタイプ宣言
void printErrorWSAStartup(int err);
int setupServer(SOCKET server, struct sockaddr_in* conf, int conf_size);
void printClientInfo(struct sockaddr_in* conf, char* request);

// エントリーポイント
int main()
{
	// Windows Sockets APIのデータを格納する変数を用意する
	WSADATA wsaData;

	// 接続検知用のソケット
	struct sockaddr_in server_conf;
	SOCKET server;
	
	// クライアントとのやり取りのためのソケット
	struct sockaddr_in client_conf;
	SOCKET client;

	// リクエスト、レスポンスメッセージ用の配列
	char request[2048];
	char response[2048];

	// WSAStartupはソケットプログラムを開始するための準備をするもので、一度だけ呼ぶ必要がある。
	// 第一引数には16bitの変数にAPIのバージョンを指定して渡す。
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (err != 0) {
		printErrorWSAStartup(err);
		return 1;
	}

	// 接続検知用のソケットを作成
	server = socket(AF_INET, SOCK_STREAM, 0);

	// サーバーの設定をする
	server_conf.sin_family = AF_INET;
	server_conf.sin_port = htons(12345);
	server_conf.sin_addr.S_un.S_addr = INADDR_ANY;
	
	if (setupServer(server, &server_conf, sizeof(server_conf)) != 0) {
		return 1;
	}

	// レスポンスメッセージを作成
	memset(response, 0, sizeof(response));
	_snprintf_s(
		response, sizeof(response), _TRUNCATE,
		"HTTP/1.1 200 OK \r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"HELLO\r\n"
	);

	// TCPクライアントからの接続要求を受け付ける
	while (1) 
	{
		// クライアントの接続待ち
		int len = sizeof(client_conf);
		client = accept(server, (struct sockaddr*)&client_conf, &len);

		// 接続が来るまで↓の処理へは進まない

		if (client == INVALID_SOCKET) {
			printf("error : %d\n", WSAGetLastError());
			break;
		}

		// リクエストメッセージを受信
		memset(request, 0, sizeof(request));
		recv(client, request, sizeof(request), 0);

		// デバッグ用に表示
		printClientInfo(&client_conf, request);

		// データ送信
		int n = send(client, response, (int)strlen(response), 0);

		if (n < 1) {
			printf("error : %d\n", WSAGetLastError());
			break;
		}

		// ソケットを閉じる
		closesocket(client);
	}

	// 終了処理
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

/*
* 接続検知用のソケットのセットアップ
* 待ち受けるポートやアドレスファミリなどを設定し接続がくるのを待てる状態にする。
*/
int setupServer(SOCKET server, struct sockaddr_in* conf, int conf_size) 
{
	if (server == INVALID_SOCKET) {
		printf("error : %d\n", WSAGetLastError());
		return 1;
	}
	
	// server用ソケットにポート番号などの情報をバインド
	if (bind(server, (struct sockaddr*)conf, conf_size) != 0) {
		printf("error : %d\n", WSAGetLastError());
		return 1;
	}

	// TCPクライアントからの接続要求を待てる状態にする
	// backlogは処理中で受け入れられない接続要求をいくつまでOSに保持してもらうかの値
	// 実際はサーバーの設定ファイルなどを用意し、そちらに定義した値を読み込んで設定すると思われる。
	int backlog = 5;
	if (listen(server, backlog) != 0) {
		printf("error : %d\n", WSAGetLastError());
		return 1;
	}

	return 0;
}

void printClientInfo(struct sockaddr_in* conf, char* request)
{
	printf("show client info:\n");
	printf("from=%s\n", inet_ntoa((*conf).sin_addr));
	printf("port=%d\n", ntohs((*conf).sin_port));
	printf("%s", request);
}