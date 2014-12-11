#include <sys/types.h>		//accept()など
#include <sys/socket.h>		//ソケット関数
#include <stdio.h>			//標準入出力
#include <netinet/in.h>		//sockaddr_inなどのソケット関係
#include <arpa/inet.h>		//インターネットアドレスなど
#include <unistd.h>
#include <string.h>

int main(){
	int s,listen_s;
	int on;
	struct sockaddr_in saddr;
	struct sockaddr_in from;
	char szBuf[1024];
	char sendBuf[1024];
	char rcvBuf[1024];
	int fromlen,nRcv;
	u_short uport;				//ポート番号

	pid_t pid;

	//ポート番号の指定
	printf("ポート番号---");
	scanf("%s",szBuf);			//文字列としてポート番号を入力
	uport = atoi(szBuf);		//文字列を数字列に変換

	listen_s = socket(AF_INET,SOCK_STREAM,0);	//異なるマシン,TDPでソケットオープン

	//エラー処理
	if(listen_s < 0){
		printf("ソケットオープンエラー\n");
		return -2;
	}else{
		printf("ソケットオープンしました\n");
	}

	//bind関数の接続制限を解除
	//これがないとaddrエラーが起きてしまう
	on = 1;
	setsockopt(listen_s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	//sockaddr_inの構造体のメンバをセット
	memset(&saddr,0,sizeof(struct sockaddr_in));		//saddrのメモリを確保
	saddr.sin_family = AF_INET;							//
	saddr.sin_port = htons(uport);						//ホスト名
	saddr.sin_addr.s_addr = INADDR_ANY;					//IPアドレス

	//
	if( bind(listen_s,(struct sockaddr *)&saddr,sizeof(saddr)) == -1){
		printf("bind Error\n");
		close(listen_s);
		return -3;
	}else{
		printf("bind関数成功です\n");
	}

	//接続待機状態
	if( listen(listen_s,5) == -1){
		printf("listen Error\n");
		close(listen_s);
		return -4;
	}else{
		printf("listen関数成功です\naccept関数で待機してます\n");
	}

	//接続の受付
	fromlen = sizeof(from);
	s = accept( listen_s , (struct sockaddr*) &from , &fromlen );
	if( s == -1 ){
		printf("accept error");
		close( listen_s );
		return -5;
	}else{
		printf("%sが接続してきました\n",inet_ntoa( from.sin_addr ));
		pid = fork();
		printf("accept関数成功です\n会話に入ります\n");
	}

	close( listen_s );

	while(1){
		if(pid == 0){
			scanf("%s" , sendBuf);
			if( strcmp( sendBuf , "s_end" ) == 0 ){
				send( s , sendBuf , (int)strlen (sendBuf) , 0 );
			}
			send( s , sendBuf , (int)strlen(sendBuf) , 0 );
		}else{
			printf("受信を待っています....\n");
			nRcv = recv( s , rcvBuf , sizeof( rcvBuf ) -1 , 0 );

			if( nRcv == -1 ){
				printf("recvエラー\n");
				break;
			}
			rcvBuf[nRcv] = '\0';

			if( strcmp( rcvBuf, "c_end" ) == 0 ){
				printf("クライアントが接続を切りました\n");
				break;
			}
			printf("受信--%s\n" , rcvBuf);
		}
	}

	close( s );
	printf("サーバー終了\n");

	return 0;
}

