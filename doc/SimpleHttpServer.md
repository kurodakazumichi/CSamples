# 簡易HTTPサーバー

## 環境

- Editor：Visual Studio 2019
- 言語：C
- 依存するライブラリ：ws2_32.lib
- サンプルプロジェクト：SimpleTcpServerDemo



## 概要

簡単なHTTPサーバーを作成する。

ブラウザ等でアクセスすると、ページに"HELLO"という文字が表示される程度のもの



## プロジェクトの作成

- 新規プロジェクトを作成する。プロジェクト名はなんでもいいけど`SimpleHttpServer`とか
- プロジェクトの種類は「空のプロジェクト」とする
- 通信機能を使いたいので、依存するライブラリに`ws2_32.lib`を追加する
- `main.c`を用意する



## ポイント

- 簡易HTTPサーバーというのは、ソケットを使った通信プログラムを作成するということである。

  ソケットというのはネットワークを通じて通信をするためのアイテムのようなもの

- サーバーはまず、誰か(クライアント)からの接続を待ち受けるためのソケットを作成する。

  このソケットにはポート番号であったり、IPv4で通信するなどの決め事を設定しておく。

- 接続を待つためのソケットを作ることで、ブラウザなどのクライアントから接続要求が来たら検知できる。

- クライアントから接続が来たら、クライアントから送られてくる情報を**受信**したり、クライアントに対して情報を**送信**するためのソケットを作成する。

- このソケットを作る事でサーバーとクライアントの間でデータのやり取りが可能になる。

- ソケットを作成したら、クライアントから送られてきた情報(リクエストメッセージ)を解析し、何らかの情報(レスポンスメッセージ)を返し、通信を切断する。



HTTPサーバーの大まかな処理の流れは以上になる。



## 実装

### お作法

WindowsのVisual Studioでソケットプログラミングをする際のお決まり

- `WinSock2.h`をインクルードする
- 最初に`WSAStartup`関数を呼び、最後に`WSACleanup`を呼ぶ



サンプル：

```c
#include <WinSock2.h>

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    /* ここに処理を書く */
    
    WSACleanup();
    return 0;
}

```



### sockaddr_in構造体

`sockaddr_in`構造体はポートやIPアドレスなど通信に必要な情報を格納する構造体である。

またここで設定するポート番号やIPアドレスはネットワークバイトオーダー (big endian) になって いないといけないため，整数をネットワークバイトオーダーに変換する `htons`関数を用いる．



### socket関数

ソケットを作成する関数

```c
SOCKET socket(
    int af,              // address family
    int type,            // socket type
    int protocol         // protocol
);
```

- address family： IPv4で通信するかIPv6で通信するかなどのルールを指定する
- type：ソケットのタイプを指定する、通信にTCPを使用するか、UDPを使用するかといった指定。
- protocol：アドレスファミリやソケットタイプによる、0を指定すると自動的に設定される(らしい)、このサンプルでは0を指定する。





## メモ

### WSA

Windows Sockets APIの略



### sin

socket address, internetの略



### WSADATA構造体

**WSADATA**構造体は、Windows Sockets仕様に関する情報を格納するために使用される。



### MAKEWORDマクロ

２つのBYTE(8ビット)からWORD(16ビット)のデータを作るマクロ

`MAKEWORD(2, 0)`とすると以下のように下位8ビットに2、上位8ビットに0が設定されたWORDが作られる。

| 上位8bit | 下位8bit |
| -------- | -------- |
| 00000000 | 00000010 |
| 0        | 2        |





## 参考

- [Geekなページ](https://www.geekpage.jp/programming/winsock/)
- [WSADATA](http://chokuto.ifdef.jp/advanced/struct/WSADATA.html)
- [sockaddr_in](http://www.fos.kuis.kyoto-u.ac.jp/le2soft/siryo-html/node16.html)
- [WSAStartup](http://chokuto.ifdef.jp/advanced/function/WSAStartup.html)
- [WSAGetLastError](https://chokuto.ifdef.jp/advanced/function/WSAGetLastError.html)
- [socket](https://chokuto.ifdef.jp/advanced/function/socket.html)
- [listen](https://chokuto.ifdef.jp/advanced/function/listen.html)
- [accept](https://chokuto.ifdef.jp/advanced/function/accept.html)
- [recv](https://chokuto.ifdef.jp/advanced/function/recv.html)
- [send](https://chokuto.ifdef.jp/advanced/function/send.html)
- [_snprintf_s](https://qiita.com/ymdymd/items/0e64b0675dfeec18255e)
- [MAKEWORD](https://yttm-work.jp/network/network_0015.html)
- [backlogとは](https://u-kipedia.hateblo.jp/entry/2015/01/01/001135)



