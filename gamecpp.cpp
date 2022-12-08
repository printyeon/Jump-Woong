#include<SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string.h>

//  ��ü����(Ŭ����)


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
	textMsg.setFont(font);             //��Ʈ
	textMsg.setCharacterSize(size);    //ũ�� 
	textMsg.setPosition(x, y);         //x, y ��ġ
	textMsg.setFillColor(color);       //��
	textMsg.setString(p); //����� ���ڿ�

	return 0;
};


int main(void)
{
	//
	//���� �Ҹ�
	SoundBuffer buffer;
	buffer.loadFromFile("sound/mainS.wav");
	Sound mainS(buffer);

	//�ٴ� �Ҹ�
	SoundBuffer buffer1;
	buffer1.loadFromFile("sound/jumpS.wav");
	Sound jumpS(buffer1);

	//���� �Դ� �Ҹ�
	SoundBuffer buffer2;
	buffer2.loadFromFile("sound/coinS.wav");
	Sound coinS(buffer2);//

	/**/
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "jump woong");
	window.setFramerateLimit(60);    //������

	//ȭ�� ��ȯ�� ���� ����//////////

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
	t1.loadFromFile("img/dino1.png");    //���� �׸��� �޾� �ɴϴ�. //
	t2.loadFromFile("img/dino2.png"); //

	//��������Ʈ�� �������
	Sprite dinoArr[2];//
	dinoArr[0] = Sprite(t1);//
	dinoArr[1] = Sprite(t2);//

	//�ٴ� �� ����ֱ� ���� ����
	int floatBottom = -300;//


	static const int DINO_Y_BOTTOM = HEIGHT + floatBottom;  //  //���� �ٴ���ġ  - t1.getSize().y
	Position dinoPos;//

	dinoPos.x = 30; //
	dinoPos.y = DINO_Y_BOTTOM; //

	//walk
	int index = 0;    //�޹� ������ �ε���//
	float frame = 0.f;//
	float frameSpeed = 0.4f;//
	const int changeCount = 5;    //�� �����ӿ� ü���� ����//
	bool stop = false;//�������� �Ȱ��� ����///
	bool stop1 = false;//�浹���� �� �ϳ� ��//

	//jump
	float gravity = 4.3f;    //�߷�. �����Ҷ� ���//
	bool isJumping = false;    //���� ������//
	bool isBottom = true;    //�ٴڿ� ���� ��Ҵ���//

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

	static const int PRICKLE_Y_BOTTOM = HEIGHT + floatBottom + 20;    //���� �ٴ�/////
	int prickleSpeed = 6;    //���� ���ǵ�

	Position pricklePos;/////////////////////////////////////////////////////////////
	pricklePos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
	pricklePos.y = PRICKLE_Y_BOTTOM;

	Position prickle2Pos;
	prickle2Pos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
	prickle2Pos.y = PRICKLE_Y_BOTTOM;

	Position cloudPos;
	cloudPos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
	cloudPos.y = 30;

	Position grassPos;
	grassPos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
	grassPos.y = 0;

	Position grass2Pos;
	grass2Pos.x = WIDTH - 20 - 1000; //ȭ�� �ۿ��� ���� ������ �ϱ�
	grass2Pos.y = 0;/////////////////////////////////////////////////////////////////




	int prickleCnt = 0;

	//���� ////////////
	Texture t6; //////// 
	t6.loadFromFile("img/coin_01.png"); /////////
	Sprite Coin(t6); /////////////

	Position CoinPos;
	CoinPos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
	CoinPos.y = PRICKLE_Y_BOTTOM;



	//�۾�  "score : " + ///////////////////////////////
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


#pragma region ������ �ݱ�, â �����ֱ�
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

			if (Keyboard::isKeyPressed(Keyboard::Enter)) //���� �Է� ����
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

			//���� ����
			if (hp == 0) {
				prickleSpeed = 0;
				textPrint(text3, font, 80, 150, 210,
					Color::White,
					"Game Over");
				textPrint(text8, font, 40, 190, 330,
					Color::White,
					"press M to main");
				stop = true;

				if (Keyboard::isKeyPressed(Keyboard::M)) //�����̽� �Է� ����
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

#pragma region �浹ó��

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

#pragma region ����
			if (Keyboard::isKeyPressed(Keyboard::Space)) //�����̽� �Է� ����
			{
				if (isBottom && !isJumping)    //�ٴ��̰�, �������� �ƴҶ�
				{
					jumpS.play();

					isJumping = true;
					isBottom = false;

				}
			}

			if (isJumping)
			{

				dinoPos.y -= gravity; //�������϶��� y���� �߷��� ���� �׷� �ö�

				stop1 = true;
			}
			else
			{
				dinoPos.y += gravity * 1.5;
				stop1 = false;
			}

			//����
			if (dinoPos.y >= DINO_Y_BOTTOM)    //�ٴڿ��� ���Ϸ� ���� �ʵ��� ����
			{
				dinoPos.y = DINO_Y_BOTTOM;
				
				isBottom = true;

			}
			if (dinoPos.y <= DINO_Y_BOTTOM - 250)    //�����ؼ� ���ַ� ���� �ʵ��� ����
			{
				isJumping = false;
			}
			if (isBottom)
				tf = false;

#pragma endregion

#pragma region �ȱ�

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

#pragma region ����

			//��ֹ� 5���� ������ ���� �������� ���� ���ǵ� ����
			if (prickleCnt == 5) {
				prickleSpeed = rand() % 10 + 6;
				gravity = 4.3f + (prickleSpeed / 4);
				prickleCnt = 0;
			}

			// ���� ����
			if (prickle2Pos.x <= 0 && pricklePos.x <= 0)
			{
				pricklePos.x = 1000;
				prickleCnt++;
			}
			else
			{
				pricklePos.x -= prickleSpeed;
			}

			//���� ����
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

#pragma region ����
			int randy = rand() % 200 + 470;
			int randx = rand() % 200 + 920;

			// ����
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

#pragma region �ΰ����ΰ͵�


			// ����
			if (cloudPos.x <= -900)
			{
				cloudPos.x = WIDTH;

			}
			else
			{
				cloudPos.x -= prickleSpeed;
			}

			// ����
			if (grassPos.x <= -1200)
			{
				grassPos.x = WIDTH;

			}
			else
			{
				grassPos.x -= prickleSpeed;
			}

			// ����
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

