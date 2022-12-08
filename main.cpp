#include <SFML/Graphics.hpp>
#include <conio.h>
#include <iostream>
#include <Windows.h>
#define WIDTH 1920
#define HEIGHT 1080
//https://blockdmask.tistory.com/371
using namespace sf;
struct Postition
{
	int x;
	int y;
};

int main(void)
{
	RenderWindow app(VideoMode(1920, 1080), "Dino Game");
	app.setFramerateLimit(60); //60프레임

		
		Texture t1;
		t1.loadFromFile("img/Walk (4).png");
		//Texture t2;
		//t2.loadFromFile("img/Walk (1).png");
		Texture t3;
		t3.loadFromFile("img/Walk (9).png");

		Sprite dinoArr[2];
		dinoArr[0] = Sprite(t1);
		dinoArr[1] = Sprite(t3);

		//공룡 바닥 위치
		static const int DINO_Y_BOTTOM = HEIGHT - t1.getSize().y - 80;
	


		Postition dinoPos;
		dinoPos.x  = 200;
		dinoPos.y  = DINO_Y_BOTTOM;

		//프레임에 따라 왼발 오른발 해주기 위한 변수 네개
		int index = 0;
		//왼발 오른발 인덱스    
		float frame = 0.f;
		
		float frameSpeed = 0.4f;
		const int changeCount = 5; //몇 프레임에 체인지 할지

		const int gravity = 2;//중력. 점프할때 사용    
		bool isJumping = false;//점프 중인지   
		bool isBottom = true; //바닥에 발이 닿았는지

		Texture t4;
		t4.loadFromFile("img/spike B.png");

		Sprite spike(t4);
		
		static const int SPIKE_Y_BOTTOM = HEIGHT - t4.getSize().y - 80;

		Postition spPos;
		spPos.x = WIDTH - 20;
		spPos.y = SPIKE_Y_BOTTOM ;
		const int spkieSpeed = 4;

		//윈도우 닫기
		while(app.isOpen())
		{
			Event e;
			while (app.pollEvent(e))
			{
				if (e.type == Event::Closed)
					app.close();
			}
		


		if(Keyboard::isKeyPressed(Keyboard::Space)) //스페이스 입력 감지
		{

			if (isBottom && !isJumping)
			{
				//printf("asdfasdfafsd");
				isJumping = true;
				isBottom = false;
			}

			

			/*
			//printf("asdfasdfafsd");
			
			*/
		}
		if (isJumping)
		{
			//dinoPos.y += gravity;
			dinoPos.y -= gravity; //점프중일때는 y에서 중력을 뺴줌 그럼 올라감
		}

		//애니메이션
		frame += frameSpeed;
		if (frame > changeCount)
		{
			frame -= changeCount;
			++index;
			if (index >= 2) { index = 0; }
		}

		//dino Position.        
		dinoArr[index].setPosition(dinoPos.x, dinoPos.y);

		



		/*
		if (isJumping)
		{
			//dinoPos.y += gravity;
			dinoPos.y -= gravity; //점프중일때는 y에서 중력을 뺴줌 그럼 올라감
		}
		else {
			dinoPos.y += gravity;
		}

		if (dinoPos.y = DINO_Y_BOTTOM)
		{
			dinoPos.y = DINO_Y_BOTTOM;
			isBottom = true;
		}


		if (dinoPos.y <= DINO_Y_BOTTOM - 100)
		{
			isJumping = false;
		}


		
		


		

		

		//tree move.        
		if(spPos.x <= 0)
		{
			spPos.x = WIDTH;
		}
		else
		{
			spPos.x -= spkieSpeed;
		}
		
		//spike Pos.        
		spike.setPosition(spPos.x, spPos.y);


		

		*/

		//draw.        
		app.clear(Color::White);
		app.draw(dinoArr[index]);
		app.draw(spike);
		app.display();


	//CursorSettings();
		}
	return 0;
}

/*
void CursorSettings()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
*/
