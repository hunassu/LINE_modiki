#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

u_short setPort();
int listenStart( u_short );
int connectClient( int );
void communication( int );

void* thread1( void* hs );
void* thread2( void* ls );

int main()
{
	pthread_t th1;

	pthread_create( &th1 , NULL , thread1 , (void *)NULL );	//スレッド作成
	pthread_join(th1,NULL);		//スレッドの終了待機

	return 0;
}

u_short setPort()
{
	u_short uport;
	char szBuf[1024];

	//ポート番号の入力
	printf("ポート番号--");
	scanf( "%s" , szBuf );
	uport = atoi( szBuf );

	return uport;
}

//接続受付の待機設定
int listenStart(u_short uport)
{
	int listen_s;
	struct sockaddr_in saddr;
	
	//接続条件
	listen_s = socket( AF_INET , SOCK_STREAM , 0 );
	if( listen_s < 0 ){
		printf( "ソケットエラー\n" );
		exit( 1 );
	}
	
	//sockadr構造体の初期化
	memset( &saddr , 0 , sizeof( struct sockaddr_in ) );
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons( uport );
	saddr.sin_addr.s_addr = INADDR_ANY;

	//binx関数
	if( bind( listen_s , (struct sockaddr*)&saddr , sizeof( saddr ) ) < -1 ){
		printf( "バインドエラー\n" );
		close( listen_s );
		exit( 1 );
	}

	//接続待ち状態
	if( listen( listen_s , 5 )  < 0 ){
		printf( "リッスンエラー\n" );
		close( listen_s );
		exit( 1 );
	}

	return listen_s;
}

//接続受付関数
int connectClient( int listen_s )
{
	int s;
	struct sockaddr_in from;
	int fromlen;
	
	//接続
	fromlen = sizeof( from );
	s = accept( listen_s , (struct sockaddr*)&from , &fromlen );
	if( s < 0 ){
		printf( "エクセプトエラー\n" );
		close( listen_s );
		exit( 1 );
	}

	return s;
}

//クライアントとの会話
void communication( int s )
{
	int nRcv;
	char szBuf[1024];
	
	while( 1 ){
		//受信処理
		printf( "受信待ち....\n" );
		nRcv = recv( s , szBuf , sizeof( szBuf ) - 1 , 0 );
		if( nRcv < 0 ){
			printf( "受信エラー\n" );
			close( s );
			exit( 1 );
		}
		szBuf[nRcv] = '\0';
		if( strcmp( szBuf , "c_end" ) == 0 ){
			printf( "クライアントが接続を切りました\n" );
			break;
		}
		printf("受信--%s\n" , szBuf );

		//送信処理
		printf("送信--");
		scanf( "%s" , szBuf );
		if( strcmp( szBuf , "s_end" ) == 0 ){
			send( s , szBuf , (int)strlen(szBuf) , 0 );
			break;
		}
		send( s , szBuf , sizeof( szBuf ) , 0 );
	}

	close( s );				//ディスクリプタを破棄
	printf( "サーバー終了\n" );
}

//クライアントの接続受付スレッド
void* thread1( void* hs )
{
	int s , listen_s;
	u_short uport;
	int i=0;
	pid_t p_pid[100];
	pthread_t th[100];

	uport = setPort();

	listen_s = listenStart( uport );

	while( 1 ){
		s = connectClient( listen_s );

		//エラー処理
		if( s < 0 ){
		}else{
			pthread_create( &th[i] , NULL , thread2 ,(void *)&s );
			printf("threadID=%d\n" , th[i]);
			i++;
		}
	}
}

//クライアントととの会話
void* thread2( void* ls )
{
	printf("aaa\n");
	printf("%d\n" ,*(int*)ls);
	communication( *(int*)ls );
}
