#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int listen_s , s;
	int fromlen , nRcv;
	struct sockaddr_in saddr;
	struct sockaddr_in from;
	char szBuf[1024];
	u_short uport;

	//ポート番号の入力
	printf("ポート番号--");
	scanf( "%s" , szBuf );
	uport = atoi( szBuf );

	//接続条件
	listen_s = socket( AF_INET , SOCK_STREAM , 0 );
	
	//sockadr構造体の初期化
	memset( &saddr , 0 , sizeof( struct sockaddr_in ) );
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons( uport );
	saddr.sin_addr.s_addr = INADDR_ANY;

	//binx関数
	bind( listen_s , (struct sockaddr*)&saddr , sizeof( saddr ) );

	//接続待ち状態
	listen( listen_s , 5 );

	//接続
	fromlen = sizeof( from );
	s = accept( listen_s , (struct sockaddr*)&from , &fromlen );

	close( listen_s );		//接続受付用のディスクリプタを破棄

	while( 1 ){
		//受信処理
		printf( "受信待ち....\n" );
		nRcv = recv( s , szBuf , sizeof( szBuf ) - 1 , 0 );
		szBuf[nRcv] = '\0';
		printf("受信--%s\n" , szBuf );

		//送信処理
		printf("送信--");
		scanf( "%s" , szBuf );
		send( s , szBuf , sizeof( szBuf ) , 0 );
	}

	close( s );				//ディスクリプタを破棄
	printf( "サーバー終了\n" );
	
	return 0;
}
