#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

int connectServer(int);
void communication(int);

int main()
{
	int connect_s;			//ディスクリプタ
	int s;

	//サーバーへの接続
	connect_s = connectServer( connect_s );

	//サーバーとの送受信
	communication( connect_s );
		
	return 0;
}	

int connectServer(int connect_s)
{
	int s;						//関数の実行結果
	struct sockaddr_in saddr;	//sockaddr構造体
	char szBuf[1024];
	struct hostent *host;		//ホスト名の保持
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
	if( connect_s < 0 ){
		printf( "ソケットエラー\n" );
		exit( 1 );
	}	
	
	//接続情報
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr = *(struct in_addr*)(host->h_addr_list[0]);

	//サーバーへ接続要求
	s = connect( connect_s , (struct sockaddr *)&saddr , sizeof( saddr ) );
	if( s < 0 ){
		printf( "ソケットオープンエラー\n" );
		close( connect_s );
		exit( 1 );
	}	
	return connect_s;
}

//
void communication(int connect_s)
{
	int nRcv;
	char szBuf[1024];
	
	while( 1 ) {
		//送信の処理
		printf( "送信--" );
		scanf( "%s" , szBuf );
		if( strcmp( szBuf , "c_end" ) == 0 ){
			send( connect_s , szBuf , (int)strlen(szBuf) , 0 );
			break;
		}
		send( connect_s , szBuf , (int)strlen(szBuf) , 0 );

		//受信の処理
		nRcv = recv( connect_s , szBuf , sizeof( szBuf ) - 1 , 0 );
		if( nRcv < 0 ){
			printf( "受信エラー\n" );
			break;
		}
		szBuf[nRcv] = '\0';
		if( strcmp( szBuf , "s_end" ) == 0 ){
			printf( "サーバーが接続を切りました\n" );
			break;
		}
		printf( "受信--%s\n" , szBuf );
	}
	close( connect_s );
}
