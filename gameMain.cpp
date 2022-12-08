#include<SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string.h>
#include <windows.h>

#define WIDTH 920
#define HEIGHT 920

using namespace std;
using namespace sf;

struct Position
{
	int x;
	int y;
};

RenderWindow window(VideoMode(WIDTH, HEIGHT), "jump woong");

//화면 전환
bool mainScene = true;
bool gameScene = false;

int index = 1;    //왼발 오른발 인덱스
int floatBottom = -300; //바닥에서 좀 띄우는 용도
const int PRICKLE_Y_BOTTOM = HEIGHT + floatBottom + 20;    //나무 바닥
float hp = 30.0f;
int prickleSpeed = 6;    //나무 스피드
float gravity = 4.3f;    //중력. 점프할때 사용
bool stop = false;//걸을건지 안걸을 건지
bool isreach = false; //

bool isCollide(Sprite s1, Sprite s2)
{
	return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}

int textPrint(Text& textMsg, Font& font, int size,
	float x, float y, const Color& color, string p)
{
	textMsg.setFont(font);             //폰트
	textMsg.setCharacterSize(size);    //크기 
	textMsg.setPosition(x, y);         //x, y 위치
	textMsg.setFillColor(color);       //색
	textMsg.setString(p); //출력할 문자열

	return 0;

};

class Player {
private:

	Texture t1;
	Texture t2;

	const int WNG_Y_BOTTOM = HEIGHT + floatBottom;
	Position wngPos;

	//walk
	float frame = 0.f;
	float frameSpeed = 0.4f;
	const int changeCount = 5;    //몇 프레임에 체인지 할지

	//jump
	bool isJumping = false;    //점프 중인지
	bool isBottom = true;    //바닥에 발이 닿았는지

	//뛰는 소리
	SoundBuffer buffer1;

public:

	//getter
	Position getWngPos() const {
		return wngPos;
	}

	Sprite wngInit() {

		t1.loadFromFile("img/dino1.png");
		Sprite wng(t1);

		return wng;
	}

	Sprite wng2Init() {

		t2.loadFromFile("img/dino2.png");
		Sprite wng2(t2);

		return wng2;
	}

	//생성과 동시에 초기화
	Player() {

		wngPos.x = 30;
		wngPos.y = WNG_Y_BOTTOM;

	}

	void jumpCheck() {
		buffer1.loadFromFile("sound/jumpS.wav");
		Sound jumpS(buffer1);

		if (isBottom && !isJumping)    //바닥이고, 점프중이 아닐때
		{
			jumpS.play();

			isJumping = true;
			isBottom = false;

		}
	}

	void jump() {

		if (isJumping)
		{
			wngPos.y -= gravity; //점프중일때는 y에서 중력을 뺴줌 그럼 올라감

			stop = true;
		}
		else
		{
			wngPos.y += gravity * 1.5;
			stop = false;
		}

		//극한
		if (wngPos.y >= WNG_Y_BOTTOM)    //바닥에서 지하로 가지 않도록 설정
		{
			wngPos.y = WNG_Y_BOTTOM;

			isBottom = true;

		}
		if (wngPos.y <= WNG_Y_BOTTOM - 250)    //점프해서 우주로 가지 않도록 설정
		{
			isJumping = false;
		}
		if(isBottom)
			isreach = false;
	}

	void walk() {
		if (stop) {
			index = 0; //인덱스 고정시켜서 멈춰있기
		}
		else {
			//wng step.
			frame += frameSpeed;
			if (frame > changeCount)
			{
				frame -= changeCount;
				++index;
				
				if (index == 2) {
					index = 0;
				}
			}
		}
	}

	void reset() {
		wngPos.y = WNG_Y_BOTTOM;
		gravity = 4.3f;
		isJumping = false;
	}

};

class Heart {
private:
	Texture t6;
	Position heartPos;

public:

	//getter
	Position getHeartPos() const {
		return heartPos;
	}

	Sprite heartInit() {
		t6.loadFromFile("img/coin_01.png");
		Sprite coin(t6);

		return coin;
	}

	Heart() {

		heartPos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
		heartPos.y = PRICKLE_Y_BOTTOM;

	}

	void move() {
		int randy = rand() % 200 + 470;
		int randx = rand() % 300 + 920;

		// 생성
		if (heartPos.x <= -30)
		{
			heartPos.x = randx;
			heartPos.y = randy;

		}
		else
		{
			heartPos.x -= prickleSpeed;
		}
	}

	void trans() {
		heartPos.x = -200;
		heartPos.y = -200;
	}

	void reset() {
		heartPos.x = WIDTH - 20;
	}
};

class Prickles {
private:

	//prickle
	Texture t3;
	//prickle 2
	Texture t7;

	Position pricklePos;
	Position prickle2Pos;


	int prickleCnt = 0;


public:

	//getter
	Position getPricklePos() const {
		return pricklePos;
	}

	Position getPrickle2Pos() const {
		return prickle2Pos;
	}


	Sprite prickleInit() {
		t3.loadFromFile("img/tree.png");
		Sprite prickle(t3);

		return prickle;
	}

	Sprite prickle2Init() {

		t7.loadFromFile("img/prickle.png");
		Sprite prickle2(t7);

		return prickle2;
	}


	Prickles() {
		
		pricklePos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
		pricklePos.y = PRICKLE_Y_BOTTOM;
		
		prickle2Pos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
		prickle2Pos.y = PRICKLE_Y_BOTTOM;
	
	}
	void move() {

		//장애물 5개를 넘을때 마다 랜덤으로 가시 스피드 조절
		if (prickleCnt == 5) {
			prickleSpeed = rand() % 10 + 6;
			gravity = 4.3f + (prickleSpeed / 4);
			prickleCnt = 0;
		}

		// 가시 생성
		if (prickle2Pos.x <= 0 && pricklePos.x <= 0)
		{
			pricklePos.x = 1000;
			prickleCnt++;
		}
		else
		{
			pricklePos.x -= prickleSpeed;
		}

		//가시 생성
		if (prickle2Pos.x <= 0 && pricklePos.x <= 0)
		{
			prickle2Pos.x = 1000;
			prickleCnt++;
		}
		else
		{
			prickle2Pos.x -= prickleSpeed;
		}
	}

	void reset() {
		prickleCnt = 0;
		prickle2Pos.x = WIDTH - 20;
		pricklePos.x = WIDTH - 20;
	}
};

int main(void)
{
	window.setFramerateLimit(60);    //프레임
	static int score = -1; //0
	
#pragma region 소리

		//메인 소리
	SoundBuffer buffer;
	buffer.loadFromFile("sound/mainS.wav");
	Sound mainS(buffer);



	//동전 먹는 소리
	SoundBuffer buffer2;
	buffer2.loadFromFile("sound/coinS.wav");
	Sound coinS(buffer2);

#pragma endregion


#pragma region 텍스트

	Text text1;
	Text text2;
	Text text3;
	Text text8;
	Text text4;
	Text text6;
	Text text7;

	string scoretxt = to_string(score);
	int x = 0, y = 0;

#pragma endregion


#pragma region 배경

	//main bg
	Texture t4;
	t4.loadFromFile("img/main.png");
	Sprite Main(t4);

	//game bg
	Texture t5;
	t5.loadFromFile("img/game.png");
	Sprite Game(t5);

#pragma endregion


	/////////////cloud
	Texture t8;
	t8.loadFromFile("img/cloud.png");
	Sprite cloud(t8);

	Position cloudPos;
	cloudPos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
	cloudPos.y = 30;

	/////////////grass
	Texture t9;
	t9.loadFromFile("img/grass.png");
	Sprite grass(t9);

	Position grassPos;
	grassPos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
	grassPos.y = 0;

	/////////////이어짐을 위한 grass2
	Texture t10;
	t10.loadFromFile("img/grass.png");
	Sprite grass2(t10);

	Position grass2Pos;
	grass2Pos.x = WIDTH - 20 - 1000; //화면 밖에서 먼저 나오게 하기
	grass2Pos.y = 0;

	//hp bar
	RectangleShape rect(Vector2f(hp*15, 30));
	rect.setPosition(95, 12);

	Player p = Player();
	Prickles pri = Prickles();
	Heart c = Heart();

	Sprite prickle = pri.prickleInit();
	Sprite prickle2 = pri.prickle2Init();

	Sprite heart = c.heartInit();

	Sprite wngArr[2];
	wngArr[0] = p.wngInit();
	wngArr[1] = p.wng2Init();

	Font font;
	font.loadFromFile("font/Starborn.ttf");
	
	mainS.play();

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
		}

		//메인 화면
		if (mainScene)
		{

			textPrint(text6, font, 40, 270, 810, Color::White, "Enter To Start");
			textPrint(text7, font, 80, 130, 210, Color::White,"jump woong");

			if (Keyboard::isKeyPressed(Keyboard::Enter)) //엔터 입력 감지
			{
				mainScene = false;
				gameScene = true;
			}

			window.clear(Color::White);
			window.draw(Main);
			window.draw(text6);
			window.draw(text7);

		}

		//인게임 화면
		if (gameScene) {

			//score
			textPrint(text2, font, 30, 680, 10, Color::White, "Score : ");
			textPrint(text1, font, 30, 840, 12, Color::White, scoretxt);

			//hp
			textPrint(text4, font, 30, 30, 10, Color::White, "Hp ");


#pragma region 게임 오버

			if (hp == 0) {
				prickleSpeed = 0;
				textPrint(text3, font, 80, 150, 210, Color::White, "Game Over");
				textPrint(text8, font, 40, 190, 330, Color::White, "press M to main");
				stop = true; //가만히 있게 하기

				if (Keyboard::isKeyPressed(Keyboard::M)) //스페이스 입력 감지
				{
					mainScene = true;
					gameScene = false;
					hp = 30;
					rect.setSize(Vector2f(hp * 15, 30));
					prickleSpeed = 6;
					score = 0;
					scoretxt = to_string(score);
					stop = false;
					
					//텍스트 잠시 안보이게 다른곳으로 옮겨 놓기
					text8.setPosition(-100, -100);
					text3.setPosition(-100, -100);

					//위치 리셋
					pri.reset();
					p.reset();
					c.reset();
				}
			}
			else {
#pragma region 충돌처리
				
				if (isCollide(wngArr[0], prickle) || isCollide(wngArr[1], prickle) || isCollide(wngArr[0], prickle2) || isCollide(wngArr[1], prickle2))
				{
					if (hp == 0) {
						hp = 0; //마이너스로 안 가도록
					}
					else {
						if (!isreach) {
							hp -= 0.5;
							isreach = true;
							rect.setSize(Vector2f(hp * 15, 30));

						}

					}
				
				}

				if (isCollide(wngArr[0], heart) || isCollide(wngArr[1], heart)) {
					coinS.play();

					c.trans(); //안보이게 숨기기
					score++;
					scoretxt = to_string(score);
				}
#pragma endregion

				//점프
				if (Keyboard::isKeyPressed(Keyboard::Space)) //스페이스 입력 감지
				{
					p.jumpCheck();
					
				}
				p.jump();

				//걷기
				p.walk();


#pragma region 부가적인것들


				// 구름 생성
				if (cloudPos.x <= -900)
				{
					cloudPos.x = WIDTH;

				}
				else
				{
					cloudPos.x -= prickleSpeed;
				}

				// 잔디 생성
				if (grassPos.x <= -1200)
				{
					grassPos.x = WIDTH;

				}
				else
				{
					grassPos.x -= prickleSpeed;
				}

				// 잔디2 생성
				if (grass2Pos.x <= -1200)
				{
					grass2Pos.x = WIDTH;

				}
				else
				{
					grass2Pos.x -= prickleSpeed;
				}
#pragma endregion

				//움직일 수 있도록
				pri.move();
				c.move();
			}




			//pricklePos
			prickle.setPosition(pri.getPricklePos().x, pri.getPricklePos().y);
			prickle2.setPosition(pri.getPrickle2Pos().x, pri.getPrickle2Pos().y);

			//coin Pos
			heart.setPosition(c.getHeartPos().x, c.getHeartPos().y);

			//cloud Position.
			cloud.setPosition(cloudPos.x, cloudPos.y);

			//grass Position.
			grass.setPosition(grassPos.x, grassPos.y);
			grass2.setPosition(grass2Pos.x, grass2Pos.y);

			//wng Pos
			wngArr[index].setPosition(p.getWngPos().x, p.getWngPos().y);

			//draw.
			window.clear(Color::White);
			window.draw(Game);

			window.draw(cloud);
			window.draw(grass);
			window.draw(grass2);
			window.draw(text1);
			window.draw(text2);
			window.draw(text4);
			window.draw(text8);
			window.draw(text3);

			window.draw(heart);
			window.draw(prickle);
			window.draw(prickle2);
			window.draw(wngArr[index]);
			window.draw(rect);




		}


		window.display();
	}

	return 0;
}


