#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>
#include <unistd.h>


#define NONE 7
#define BLUE 0
#define YELLOW 1
#define RED 2
#define GREEN 3
#define MAGENTA 6
#define TRUE 1
#define FALSE 0
#define true 1
#define false 0
#define PI 3.14159 



int N;

char score_string[30];
char life_string[30];
char cubes_string[30];

GLfloat light_position0[4];
GLfloat light_position1[4];
GLfloat light_position2[4];
GLfloat light_position3[4];

typedef struct grid{
	int ***board;
} Grid;

typedef struct player {
	float x, y, z;
	int score;
	int life;
	int cubes;
} Player;



Grid grid;

Player player ={ 5.0, 1.0,5.0,50,3,10}; 

float lx=0.0,lz=1.0;
float angle1 =0.0; 



void initBoard(char* length){

	int i,j,k;

	N = atoi(length);

	grid.board = (int***)malloc(N*sizeof(int**));

	for(i=0; i<N; i++){
		
		grid.board[i] = (int**)malloc(N*sizeof(int*));

		for(j=0; j<N; j++){
	
			grid.board[i][j] = (int*)malloc(N*sizeof(int));			
		}
	}

	
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			for(k=0; k<N; k++){
				grid.board[i][j][k] = NONE;	
			}		
		}
	}


	printf("N: %d\n",N);
	player.x = round(N/2);
	player.y = 1; 
	player.z = round(N/2);
}	
	



int validMove(float dx, float dy, float dz){
	float fraction=0.8f;

	if( (player.x + dx) < 0) return 0;
	if( (player.x + dx) >=N) return 0;
	if( (player.y + dy) <0)  return 0;
	if( (player.y + dy) >=N) return 0;
	if( (player.z + dz) <0)  return 0;
	if( (player.z + dz) >=N) return 0;

	int tx = player.x+dx;
	int ty = player.y+dy;
	int tz = player.z+dz;

	if(grid.board[tx][ty][tz]!=NONE) return -1;
	
	return 1;
}

void dropPlayer(){

	int tx = player.x;
	int ty = player.y;
	int tz = player.z;
	int i=0;

	while(ty>=1 && grid.board[tx][ty-1][tz]==NONE){
		ty -=1;
		i++;

			if(i>=2){
				if (((player.score-5*i)>=0)&&(ty!=0)){
					player.score-=5;
					
				}else if(ty!=0) {
					player.life -=1;
				}		
					
			}else if(i>=1){
				if ((player.life-1)>=0&&(ty==0)){
					player.life-=1;
				}
			}
	}

	player.y = ty;

	
}


void placeCube(int dx, int dy, int dz){

	int i;
	
	if(player.cubes-1 >= 0){

		if(grid.board[dx][dy][dz] == NONE){
	
			grid.board[dx][dy][dz] = rand()%4;	
			player.cubes-=1;
		}else{
			for(i=0; i<N-3; i++){

				if(grid.board[dx][i+1][dz] == NONE){

					grid.board[dx][i+1][dz] = rand()%4;
					player.cubes-=1;
					break;
				}		
			}
		}
	}		
}


void pushCube(){

	int moveitem;
	int dx,dz;
	int nx,ny,nz;
	int temp;
	
	nx=player.x+lx;
	ny=player.y;
	nz=player.z+lz;

	if(angle1>=PI/4 && angle1<3*PI/4.0){
		dx = -1;
		dz = 0;
	}
	else if(angle1>=3*PI/4.0 && angle1<5.0*PI/4.0){
		dx = 0;
		dz = -1;
	}
	else if(angle1>=5.0*PI/4.0 && angle1<7.0*PI/4.0){
		dx = 1;
		dz = 0;
	}
	else {
		dx = 0;
		dz = 1;
	}
	


	nx=(int) player.x+dx;
	ny= (int) player.y;
	nz= (int) player.z+dz;
	if(grid.board[nx][ny][nz] != NONE && grid.board[nx][ny][nz]){
		
		moveitem = grid.board[nx][ny][nz];
		grid.board[nx][ny][nz] = NONE;
		nx +=dx;
		nz +=dz;
		while(nx>=0 && nz>=0 && nx<N && nz<N && grid.board[nx][ny][nz]!=NONE){
			temp = grid.board[nx][ny][nz];
			grid.board[nx][ny][nz] = moveitem;
			moveitem = temp;	
			nx +=dx;
			nz +=dz;
			
		}
		grid.board[nx][ny][nz] = moveitem;
	} 	

}

void jumpPlayer(){

	int tx=player.x+lx;
	int ty=player.y;
	int tz=player.z+lz;
	
	if((ty<N-1)&&(grid.board[tx][ty][tz]!=NONE)&&(grid.board[tx][ty+1][tz]==NONE)){
		ty+=1;
		tz+=1;
		player.score+=5;
		player.y+=1;
		player.z+=lz;
	}
	
	if(ty==N-2){
		player.score+=100;
		player.life+=1;
	}
}


int dropCubes(){

	int i,j,k;
	int flag=0;
	for(i=0; i<N; i++){
		for(j=1; j<N; j++){
			for(k=0; k<N; k++){
				if(grid.board[i][j][k] != NONE && grid.board[i][j-1][k] == NONE && grid.board[i][0][k] !=NONE ){
					grid.board[i][j-1][k] = grid.board[i][j][k];
					grid.board[i][j][k] = NONE;

					flag=1;
				}else if(grid.board[i][j][k] != NONE && grid.board[i][j-1][k] == NONE && grid.board[i][0][k] ==NONE){
					grid.board[i][j][k] =NONE;

					flag=1;									
				}
			}		
		}
	}

	return flag;
}

void timerFunc(int x){

		if(dropCubes()==1){
			glutTimerFunc(500,timerFunc, 0);
		}
}


void keyPressFunc(unsigned char key, int x, int y){

	float fraction = 0.8f;
	float fraction2 = 0.3f;
	int flag,nx,ny,nz;

	if (key==27)
		exit(0);
	

	if(key=='w'|| key== 'W'){
		if((flag=validMove(lx*fraction,0,lz*fraction))==1){
			player.x += lx *fraction;
			player.z += lz *fraction;
			dropPlayer();
		}
		else if(flag==-1){
			jumpPlayer();
		}else{
			printf("%d\t%.2f %.2f %.2f %.2f\n",flag,player.x,player.z,lx*fraction,lz*fraction);
		}
	}else if(key=='a' || key == 'A'){
		angle1 -= fraction2;
		lx = -sin(angle1);
		lz = cos(angle1);
		if(angle1<0) angle1+= 2*PI;

	}else if(key =='d' || key == 'D'){
		angle1 += fraction2;
		lx = -sin(angle1);
		lz = cos(angle1);

		if(angle1>=2*PI) angle1-=2*PI;

	}else if(key=='s'|| key== 'S'){
		angle1 += PI;
		lx = -sin(angle1);
		lz = cos(angle1);
		if(angle1>=2*PI) angle1-=2*PI;

	}else if(key==' '){
	
		int nx=player.x;
		int ny=player.y-1;
		int nz=player.z;

		if(player.y == 0) return;

		if(grid.board[nx][ny][nz]==RED ){
			if ((player.score-5)>=0){
				player.score-=5;
				player.cubes+=1;
				grid.board[nx][ny][nz]=YELLOW;
			}
		}else if(grid.board[nx][ny][nz]==BLUE){
			if ((player.score-5)>=0){
				player.score-=5;
				player.cubes+=0;

			}
		}else if(grid.board[nx][ny][nz]==YELLOW){
			if ((player.score-5)>=0){
				player.score-=5;
				player.cubes+=1;
				grid.board[nx][ny][nz]=BLUE;	
			}
		}else if(grid.board[nx][ny][nz]==GREEN){
			if ((player.score-5)>=0){
				player.score-=5;
				player.cubes+=1;
				grid.board[nx][ny][nz]=RED;
			}
		}
	
	}else if (key=='q'||key=='Q'){
			int nx=player.x+lx;
			int ny=player.y;
			int nz=player.z+lz;	

			if(grid.board[nx][ny][nz] != MAGENTA){ 
				grid.board[nx][ny][nz] = NONE;

			}

	}else if(key=='e'||key=='E'){

			int nx=player.x +lx;
			int ny=player.y;
			int nz=player.z + lz;
			int i;


			if(player.y == 0) return;			
	
			if (player.score-20>=0){
				
				if((grid.board[nx][ny-1][nz] != NONE && grid.board[nx][ny-1][nz] != MAGENTA && player.y!=0)){	
					
					grid.board[nx][ny-1][nz] = NONE;



					for(i=0; i<N-2; i++){				
							grid.board[nx][ny+i][nz] = NONE;
					}

					player.score -= 20;				
					player.life += 1;
					
				}else if((grid.board[nx][ny-1][nz] != NONE && grid.board[nx][ny-1][nz] == MAGENTA)){	

					for(i=0; i<N-2; i++){				
							grid.board[nx][ny+i][nz] = NONE;
					}
					player.score -= 20;				
					player.life += 1;					
					
				}
			}

	}else if(key=='r'||key=='R'){
		if(dropCubes()==1){
			glutTimerFunc(500,timerFunc, 0);
			player.score +=10;			
		}
	}
}

void mouse_button(int button, int state, int x, int y) {

	int nx,ny,nz;

	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){

		nx=player.x+lx;
		ny=player.y;
		nz=player.z+lz;
	
		placeCube(nx,ny,nz);

	}else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){

		pushCube();

	}
}

void drawText(char *string, float x, float y) {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	gluOrtho2D(0, 800, 800, 0);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glLoadIdentity();

	while(*string != '\0') {
		glRasterPos2f(x,y);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string);
		x += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24,*string);
		string++;	
	}

	glPopMatrix();
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void drawBoard(){

	int i,j,k;

	glTranslated(0.5,0.5,0.5);
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			for(k=0; k<N; k++){
				if(grid.board[i][j][k]!=NONE){
					if(grid.board[i][j][k]==RED){
						glColor3f(1,0,0);					
					}
					else if(grid.board[i][j][k]==BLUE){
						glColor3f(0,0,1);
					}else if(grid.board[i][j][k]==GREEN){
						glColor3f(0,1,0); 
					}else if(grid.board[i][j][k]==YELLOW){
						glColor3f(1,1,0);
					}else if(grid.board[i][j][k]== MAGENTA){
						glColor3f(1,0,1);
					}					
						
					glTranslated(i,j,k);
					glutSolidCube(1);
					glTranslated(-i,-j,-k);
				}	
				
			}
		}
	}	
	glTranslated(-0.5,-0.5,-0.5);
}


void drawPlayer(){

	glColor3f(0,1,1);
	glPushMatrix();
	glTranslated(player.x,player.y+0.5,player.z);


	glRotatef(-90,0,1,0);
	glRotatef(-(angle1*180/PI),0,1,0);
	glutSolidTeapot(0.3);	

	glPopMatrix();
}

void renderScene(){	

	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(player.x-1.5*lx,player.y+1.5,player.z-1.5*lz,  player.x+lx,player.y+1,player.z+lz,  0.0,1.0,0.0);


	
	glBegin(GL_QUADS);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(N,0.0f,0.0);
		glVertex3f(N,0.0,N);
		glVertex3f(0.0,0.0,N);
	glEnd();


	glNormal3f(0,0,1);
	glColor3f(0.6,0.6,0.6);	
	glBegin(GL_QUADS);
		glVertex3f(0,0,0);
		glVertex3f(N,0,0);
		glVertex3f(N,N,0);
		glVertex3f(0,N,0);
	glEnd();

	glNormal3f(1,0,0);
	glColor3f(0.6,0.6,0.6);	
	glBegin(GL_QUADS);
		glVertex3f(0,0,0);
		glVertex3f(0,0,N);
		glVertex3f(0,N,N);
		glVertex3f(0,N,0);
	glEnd();
	
	
	drawPlayer();		

	drawBoard();


	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

	sprintf(score_string, "Score = %d", player.score);
	sprintf(life_string, "Lifes = %d", player.life);
	sprintf(cubes_string, "Cubes = %d", player.cubes);	

	drawText(score_string, 20, 20);	//gia thn thesi
	drawText(life_string, 20, 50);	
	drawText(cubes_string, 20, 80);


	glutSwapBuffers();
}


void init(){

	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glEnable(GL_LIGHT2);

	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glEnable(GL_LIGHT3);

	light_position0[0] = N;
	light_position0[1] = N;
	light_position0[2] = N;
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glEnable(GL_LIGHT0);


	light_position1[0] = 0;
	light_position1[1] = N;
	light_position1[2] = N;
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glEnable(GL_LIGHT1);


	light_position2[0] = 0;
	light_position2[1] = N;
	light_position2[2] = 0;
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glEnable(GL_LIGHT2);




	light_position3[0] = N;
	light_position3[1] = N;
	light_position3[2] = 0;
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glEnable(GL_LIGHT3); 




	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
}

void changeSize(int w, int h){



	if(h==0)
		h=1;
	float ratio = 1.0* w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,w,h);
	gluPerspective(45,ratio,0.1,1000);
	glMatrixMode(GL_MODELVIEW);

	init();
}



int main(int argc,char** argv){
	int i,z;
	if(argc != 2){
		printf("dwse to megethos san eisodo!\n");
		exit(0);
	} 



	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(150,150);
	glutCreateWindow("MINECRAFT");
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyPressFunc);
	glutMouseFunc(mouse_button);                    
	glutIdleFunc(renderScene);


	initBoard(argv[1]);

	
	for (i=0;i<N;i++){
		for(z=0;z<N;z++){
			grid.board[i][0][z] = rand()%4;
		}
		
	} 
	grid.board[N/2][0][N/2] = MAGENTA;

	glutMainLoop();
	return 0;
}
