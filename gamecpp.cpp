#include<SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string.h>

//  객체지향(클래스)


using namespace std;
using namespace sf;

#define WIDTH 920
#define HEIGHT 920
struct Position
{
	int x;
	int y;
};

/////////////////////



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


int main(void)
{
	//
	//메인 소리
	SoundBuffer buffer;
	buffer.loadFromFile("sound/mainS.wav");
	Sound mainS(buffer);

	//뛰는 소리
	SoundBuffer buffer1;
	buffer1.loadFromFile("sound/jumpS.wav");
	Sound jumpS(buffer1);

	//동전 먹는 소리
	SoundBuffer buffer2;
	buffer2.loadFromFile("sound/coinS.wav");
	Sound coinS(buffer2);//

	/**/
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "jump woong");
	window.setFramerateLimit(60);    //프레임

	//화면 전환을 위한 변수//////////

	bool mainScene = true;
	bool gameScene = false;

	Texture t4;
	t4.loadFromFile("img/main.png");
	Sprite Main(t4);////////////////////////////



#pragma region gameSceneVar

	static int score = -1; //0////////////

	//bg
	Texture t5;
	t5.loadFromFile("img/game.png");
	Sprite Game(t5);///////////

	//dino
	Texture t1; //
	Texture t2; //
	t1.loadFromFile("img/dino1.png");    //실제 그림을 받아 옵니다. //
	t2.loadFromFile("img/dino2.png"); //

	//스프라이트로 만들어줌
	Sprite dinoArr[2];//
	dinoArr[0] = Sprite(t1);//
	dinoArr[1] = Sprite(t2);//

	//바닥 좀 띄워주기 위한 변수
	int floatBottom = -300;//


	static const int DINO_Y_BOTTOM = HEIGHT + floatBottom;  //  //공룡 바닥위치  - t1.getSize().y
	Position dinoPos;//

	dinoPos.x = 30; //
	dinoPos.y = DINO_Y_BOTTOM; //

	//walk
	int index = 0;    //왼발 오른발 인덱스//
	float frame = 0.f;//
	float frameSpeed = 0.4f;//
	const int changeCount = 5;    //몇 프레임에 체인지 할지//
	bool stop = false;//걸을건지 안걸을 건지///
	bool stop1 = false;//충돌나서 걍 하나 더//

	//jump
	float gravity = 4.3f;    //중력. 점프할때 사용//
	bool isJumping = false;    //점프 중인지//
	bool isBottom = true;    //바닥에 발이 닿았는지//

	//prickle
	Texture t3;
	t3.loadFromFile("img/tree.png");
	Sprite prickle(t3);

	//prickle 2
	Texture t7;
	t7.loadFromFile("img/prickle.png");
	Sprite prickle2(t7);

	//cloud//////////////////////////////////////
	Texture t8;
	t8.loadFromFile("img/cloud.png");
	Sprite cloud(t8);

	//grass
	Texture t9;
	t9.loadFromFile("img/grass.png");
	Sprite grass(t9);

	Texture t10;
	t10.loadFromFile("img/grass.png");
	Sprite grass2(t10);////////////////////////////

	static const int PRICKLE_Y_BOTTOM = HEIGHT + floatBottom + 20;    //나무 바닥/////
	int prickleSpeed = 6;    //나무 스피드

	Position pricklePos;/////////////////////////////////////////////////////////////
	pricklePos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
	pricklePos.y = PRICKLE_Y_BOTTOM;

	Position prickle2Pos;
	prickle2Pos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
	prickle2Pos.y = PRICKLE_Y_BOTTOM;

	Position cloudPos;
	cloudPos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
	cloudPos.y = 30;

	Position grassPos;
	grassPos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
	grassPos.y = 0;

	Position grass2Pos;
	grass2Pos.x = WIDTH - 20 - 1000; //화면 밖에서 먼저 나오게 하기
	grass2Pos.y = 0;/////////////////////////////////////////////////////////////////




	int prickleCnt = 0;

	//동전 ////////////
	Texture t6; //////// 
	t6.loadFromFile("img/coin_01.png"); /////////
	Sprite Coin(t6); /////////////

	Position CoinPos;
	CoinPos.x = WIDTH - 20; //화면 밖에서 먼저 나오게 하기
	CoinPos.y = PRICKLE_Y_BOTTOM;



	//글씨  "score : " + ///////////////////////////////
	Text text1;
	Text text2;

	Text text3;
	Text text8;

	Text text4;
	Text text5;

	Text text6;
	Text text7;
	Uint8 r = 0, g = 0, b = 0;

	int hp = 51;//50 //
	bool tf = false; //


	string scoretxt = to_string(score);//
	string hptxt = to_string(hp);//
	int x = 0, y = 0;//



#pragma endregion


#pragma region 윈도우 닫기, 창 보여주기
	mainS.play();

	Font font;
	font.loadFromFile("font/Starborn.ttf");

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


#pragma endregion

		if (mainScene)
		{

			textPrint(text6, font, 40, 270, 810,
				Color::White,
				"Enter To Start");

			textPrint(text7, font, 80, 130, 210,
				Color::White,
				"jump woong");

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

		if (gameScene) {

			//게임 오버
			if (hp == 0) {
				prickleSpeed = 0;
				textPrint(text3, font, 80, 150, 210,
					Color::White,
					"Game Over");
				textPrint(text8, font, 40, 190, 330,
					Color::White,
					"press M to main");
				stop = true;

				if (Keyboard::isKeyPressed(Keyboard::M)) //스페이스 입력 감지
				{
					mainScene = true;
					gameScene = false;
					hp = 51;
					hptxt = to_string(hp);
					prickleSpeed = 6;
					score = 0;
					scoretxt = to_string(score);

					text8.setPosition(-100, -100);
					text3.setPosition(-100, -100);
					stop = false;

				}
			}

			//score
			textPrint(text2, font, 30, 370, 10,
				Color::White,
				"Score : ");

			textPrint(text1, font, 30, 530, 12,
				Color::White,
				scoretxt);

			//hp
			textPrint(text4, font, 30, 5, 10,
				Color::White,
				"Hp : ");

			textPrint(text5, font, 30, 95, 12,
				Color::White,
				hptxt);

#pragma region 충돌처리

			if (isCollide(dinoArr[0], prickle) || isCollide(dinoArr[1], prickle) || isCollide(dinoArr[0], prickle2) || isCollide(dinoArr[1], prickle2))
			{

				if (hp == 0) {
					tf = true;
					hp = 0;
				}
				else if (!tf) {
					hp--;
					hptxt = to_string(hp);
					tf = true;
				}
			}

#pragma endregion

#pragma region 점프
			if (Keyboard::isKeyPressed(Keyboard::Space)) //스페이스 입력 감지
			{
				if (isBottom && !isJumping)    //바닥이고, 점프중이 아닐때
				{
					jumpS.play();

					isJumping = true;
					isBottom = false;

				}
			}

			if (isJumping)
			{

				dinoPos.y -= gravity; //점프중일때는 y에서 중력을 뺴줌 그럼 올라감

				stop1 = true;
			}
			else
			{
				dinoPos.y += gravity * 1.5;
				stop1 = false;
			}

			//극한
			if (dinoPos.y >= DINO_Y_BOTTOM)    //바닥에서 지하로 가지 않도록 설정
			{
				dinoPos.y = DINO_Y_BOTTOM;
				
				isBottom = true;

			}
			if (dinoPos.y <= DINO_Y_BOTTOM - 250)    //점프해서 우주로 가지 않도록 설정
			{
				isJumping = false;
			}
			if (isBottom)
				tf = false;

#pragma endregion

#pragma region 걷기

			if (stop) {
				index = 0;
			}
			else if (stop1){
				index = 0;
			}
			else {
				//dino step.
				frame += frameSpeed;
				if (frame > changeCount)
				{
					frame -= changeCount;
					++index;
					if (index == 2) { index = 0; }

				}
			}
#pragma endregion

#pragma region 가시

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

#pragma endregion

#pragma region 동전
			int randy = rand() % 200 + 470;
			int randx = rand() % 200 + 920;

			// 생성
			if (CoinPos.x <= -30)
			{
				CoinPos.x = randx;
				CoinPos.y = randy;

			}
			else
			{
				CoinPos.x -= prickleSpeed;
			}

			if (isCollide(dinoArr[0], Coin)) {
				coinS.play();

				CoinPos.x = -200;
				CoinPos.y = -200;
				score++;
				scoretxt = to_string(score);
			}
			else if (isCollide(dinoArr[1], Coin))
			{
				CoinPos.x = -200;
				CoinPos.y = -200;
				score++;
				scoretxt = to_string(score);

			}

#pragma endregion

#pragma region 부가적인것들


			// 생성
			if (cloudPos.x <= -900)
			{
				cloudPos.x = WIDTH;

			}
			else
			{
				cloudPos.x -= prickleSpeed;
			}

			// 생성
			if (grassPos.x <= -1200)
			{
				grassPos.x = WIDTH;

			}
			else
			{
				grassPos.x -= prickleSpeed;
			}

			// 생성
			if (grass2Pos.x <= -1200)
			{
				grass2Pos.x = WIDTH;

			}
			else
			{
				grass2Pos.x -= prickleSpeed;
			}

#pragma endregion


			//prickle Position.
			prickle.setPosition(pricklePos.x, pricklePos.y);
			prickle2.setPosition(prickle2Pos.x, prickle2Pos.y);

			//coin Pos
			Coin.setPosition(CoinPos.x, CoinPos.y);

			//dino Position.
			dinoArr[index].setPosition(dinoPos.x, dinoPos.y);

			//cloud Position.
			cloud.setPosition(cloudPos.x, cloudPos.y);

			//grass Position.
			grass.setPosition(grassPos.x, grassPos.y);
			grass2.setPosition(grass2Pos.x, grass2Pos.y);

			//draw.
			window.clear(Color::White);
			window.draw(Game);


			window.draw(cloud);
			window.draw(grass);
			window.draw(grass2);
			window.draw(text1);
			window.draw(text2);
			window.draw(text4);
			window.draw(text5);
			window.draw(text8);
			window.draw(text3);

			window.draw(Coin);
			window.draw(prickle2);
			window.draw(prickle);
			window.draw(dinoArr[index]);

		}

		window.display();
	}
	return 0;
}

