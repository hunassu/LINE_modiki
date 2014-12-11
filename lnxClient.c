#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

int main()
{
	int connect_s , s;			//ディスクリプタ
	int addrlen , nRcv;
	int result;					//関数の実行結果
	struct sockaddr_in saddr;	//sockaddr構造体
	char szBuf[1024];
	struct hostent *host;
	u_short uport;				//ポート番号

	//ポート番号の入力
	printf("使用するポート---");
	scanf( "%s" , szBuf );
	uport = atoi( szBuf );

	//ホスト名の入力
	printf("接続先のホスト名---");
	scanf( "%s" , szBuf );
	host = gethostbyname( szBuf );

	//ソケットの生成
	connect_s = socket( AF_INET , SOCK_STREAM , 0 );
	
	//接続情報
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr = *(struct in_addr*)(host->h_addr_list[0]);

	//サーバーへ接続要求
	s = connect( connect_s , (struct sockaddr *)&saddr , sizeof( saddr ) );

	//送信の処理
	printf( "送信--" );
	scanf( "%s" , szBuf );
	send ( connect_s , szBuf , sizeof(szBuf) , 0 );

	//受信の処理
	nRcv = recv( connect_s , szBuf , sizeof( szBuf ) - 1 , 0 );
	szBuf[nRcv] = '\0';
	printf( "受信--%s\n" , szBuf );

	close( connect_s );
	
	return 0;
}	

