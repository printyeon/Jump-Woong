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

//ȭ�� ��ȯ
bool mainScene = true;
bool gameScene = false;

int index = 1;    //�޹� ������ �ε���
int floatBottom = -300; //�ٴڿ��� �� ���� �뵵
int PRICKLE_Y_BOTTOM = HEIGHT + floatBottom + 20;    //���� �ٴ�
int hp = 30;
int prickleSpeed = 6;    //���� ���ǵ�
float gravity = 4.3f;    //�߷�. �����Ҷ� ���
bool stop = false;//�������� �Ȱ��� ����
bool isreach = false; //

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

class Player {
private:

	Texture t1;
	Texture t2;

	int DINO_Y_BOTTOM = HEIGHT + floatBottom;
	Position dinoPos;

	//walk
	float frame = 0.f;
	float frameSpeed = 0.4f;
	const int changeCount = 5;    //�� �����ӿ� ü���� ����

	//jump
	bool isJumping = false;    //���� ������
	bool isBottom = true;    //�ٴڿ� ���� ��Ҵ���

	//�ٴ� �Ҹ�
	SoundBuffer buffer1;

public:

	//getter
	Position getDinoPos() const {
		return dinoPos;
	}

	Sprite dinoInit() {

		t1.loadFromFile("img/dino1.png");
		Sprite dino(t1);

		return dino;
	}

	Sprite dino2Init() {

		t2.loadFromFile("img/dino2.png");
		Sprite dino2(t2);

		return dino2;
	}

	//������ ���ÿ� �ʱ�ȭ
	Player() {

		dinoPos.x = 30;
		dinoPos.y = DINO_Y_BOTTOM;

	}

	void jumpCheck() {
		buffer1.loadFromFile("sound/jumpS.wav");
		Sound jumpS(buffer1);

		if (isBottom && !isJumping)    //�ٴ��̰�, �������� �ƴҶ�
		{
			jumpS.play();

			isJumping = true;
			isBottom = false;

		}
	}

	void jump() {

		if (isJumping)
		{
			dinoPos.y -= gravity; //�������϶��� y���� �߷��� ���� �׷� �ö�

			stop = true;
		}
		else
		{
			dinoPos.y += gravity * 1.5;
			stop = false;
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
		if(isBottom)
			isreach = false;
	}

	void walk() {
		if (stop) {
			index = 0; //�ε��� �������Ѽ� �����ֱ�
		}
		else {
			//dino step.
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
		dinoPos.y = DINO_Y_BOTTOM;
		bool isJumping = false;
	}

};

class Coin {
private:
	Texture t6;
	Position coinPos;

public:

	//getter
	Position getCoinPos() const {
		return coinPos;
	}

	Sprite coinInit() {
		t6.loadFromFile("img/coin_01.png");
		Sprite coin(t6);

		return coin;
	}

	Coin() {

		coinPos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
		coinPos.y = PRICKLE_Y_BOTTOM;

	}

	void move() {
		int randy = rand() % 200 + 470;
		int randx = rand() % 300 + 920;

		// ����
		if (coinPos.x <= -30)
		{
			coinPos.x = randx;
			coinPos.y = randy;

		}
		else
		{
			coinPos.x -= prickleSpeed;
		}
	}

	void trans() {
		coinPos.x = -200;
		coinPos.y = -200;
	}

	void reset() {
		coinPos.x = WIDTH - 20;
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
		
		pricklePos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
		pricklePos.y = PRICKLE_Y_BOTTOM;
		
		prickle2Pos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
		prickle2Pos.y = PRICKLE_Y_BOTTOM;
	
	}
	void move() {

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
	}

	void reset() {
		prickleCnt = 0;
		prickle2Pos.x = WIDTH - 20;
		pricklePos.x = WIDTH - 20;
	}
};

int main(void)
{
	window.setFramerateLimit(60);    //������
	static int score = -1; //0

#pragma region �Ҹ�

		//���� �Ҹ�
	SoundBuffer buffer;
	buffer.loadFromFile("sound/mainS.wav");
	Sound mainS(buffer);



	//���� �Դ� �Ҹ�
	SoundBuffer buffer2;
	buffer2.loadFromFile("sound/coinS.wav");
	Sound coinS(buffer2);

#pragma endregion


#pragma region �ؽ�Ʈ

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


#pragma region ���

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
	cloudPos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
	cloudPos.y = 30;

	/////////////grass
	Texture t9;
	t9.loadFromFile("img/grass.png");
	Sprite grass(t9);

	Position grassPos;
	grassPos.x = WIDTH - 20; //ȭ�� �ۿ��� ���� ������ �ϱ�
	grassPos.y = 0;

	/////////////�̾����� ���� grass2
	Texture t10;
	t10.loadFromFile("img/grass.png");
	Sprite grass2(t10);

	Position grass2Pos;
	grass2Pos.x = WIDTH - 20 - 1000; //ȭ�� �ۿ��� ���� ������ �ϱ�
	grass2Pos.y = 0;

	//hp bar
	RectangleShape rect(Vector2f(hp*15, 30));
	rect.setPosition(95, 12);

	Player p = Player();
	Prickles pri = Prickles();
	Coin c = Coin();

	Sprite prickle = pri.prickleInit();
	Sprite prickle2 = pri.prickle2Init();

	Sprite coin = c.coinInit();

	Sprite dinoArr[2];
	dinoArr[0] = p.dinoInit();
	dinoArr[1] = p.dino2Init();

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

		//���� ȭ��
		if (mainScene)
		{

			textPrint(text6, font, 40, 270, 810, Color::White, "Enter To Start");
			textPrint(text7, font, 80, 130, 210, Color::White,"jump woong");

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

		//�ΰ��� ȭ��
		if (gameScene) {

			//score
			textPrint(text2, font, 30, 680, 10, Color::White, "Score : ");
			textPrint(text1, font, 30, 840, 12, Color::White, scoretxt);

			//hp
			textPrint(text4, font, 30, 30, 10, Color::White, "Hp ");


#pragma region ���� ����

			if (hp == 0) {
				prickleSpeed = 0;
				textPrint(text3, font, 80, 150, 210, Color::White, "Game Over");
				textPrint(text8, font, 40, 190, 330, Color::White, "press M to main");
				stop = true; //������ �ְ� �ϱ�

				if (Keyboard::isKeyPressed(Keyboard::M)) //�����̽� �Է� ����
				{
					mainScene = true;
					gameScene = false;
					hp = 30;
					rect.setSize(Vector2f(hp * 15, 30));
					prickleSpeed = 6;
					score = 0;
					scoretxt = to_string(score);
					stop = false;
					
					//�ؽ�Ʈ ��� �Ⱥ��̰� �ٸ������� �Ű� ����
					text8.setPosition(-100, -100);
					text3.setPosition(-100, -100);

					//��ġ ����
					pri.reset();
					p.reset();
					c.reset();
				}
			}
			else {
#pragma region �浹ó��
				
					if (isCollide(dinoArr[0], prickle) || isCollide(dinoArr[1], prickle) || isCollide(dinoArr[0], prickle2) || isCollide(dinoArr[1], prickle2))
					{
						//if(p.getDinoPos().y > 500)
							//printf("sdfafasf");

						
						if (hp == 0) {
							hp = 0; //���̳ʽ��� �� ������
						}
						else {
							if (!isreach) {
									hp--;
									isreach = true;
									rect.setSize(Vector2f(hp * 15, 30));
								
						}
						
					}
				}

				if (isCollide(dinoArr[0], coin) || isCollide(dinoArr[1], coin)) {
					coinS.play();

					c.trans(); //�Ⱥ��̰� �����
					score++;
					scoretxt = to_string(score);
				}
#pragma endregion

				//����
				if (Keyboard::isKeyPressed(Keyboard::Space)) //�����̽� �Է� ����
				{
					p.jumpCheck();
				}
				p.jump();

				//�ȱ�
				p.walk();


#pragma region �ΰ����ΰ͵�


				// ���� ����
				if (cloudPos.x <= -900)
				{
					cloudPos.x = WIDTH;

				}
				else
				{
					cloudPos.x -= prickleSpeed;
				}

				// �ܵ� ����
				if (grassPos.x <= -1200)
				{
					grassPos.x = WIDTH;

				}
				else
				{
					grassPos.x -= prickleSpeed;
				}

				// �ܵ�2 ����
				if (grass2Pos.x <= -1200)
				{
					grass2Pos.x = WIDTH;

				}
				else
				{
					grass2Pos.x -= prickleSpeed;
				}
#pragma endregion

				//������ �� �ֵ���
				pri.move();
				c.move();
			}




			//pricklePos
			prickle.setPosition(pri.getPricklePos().x, pri.getPricklePos().y);
			prickle2.setPosition(pri.getPrickle2Pos().x, pri.getPrickle2Pos().y);

			//coin Pos
			coin.setPosition(c.getCoinPos().x, c.getCoinPos().y);

			//cloud Position.
			cloud.setPosition(cloudPos.x, cloudPos.y);

			//grass Position.
			grass.setPosition(grassPos.x, grassPos.y);
			grass2.setPosition(grass2Pos.x, grass2Pos.y);

			//dino Pos
			dinoArr[index].setPosition(p.getDinoPos().x, p.getDinoPos().y);

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

			window.draw(coin);
			window.draw(prickle);
			window.draw(prickle2);
			window.draw(dinoArr[index]);
			window.draw(rect);




		}


		window.display();
	}

	return 0;
}

