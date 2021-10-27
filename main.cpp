/**
 * TETRIS ->
 * cosas que faltan:
 * UI (score, lines, level) -> lado derecho de la pantalla.
 * definir Level y Piezas por separado (ahora mismo es un bardo)
 * Definir game over , restart , una pantalla de titulo.
 */

#include <time.h>
#include <SFML/Graphics.hpp>
const int m_roomWidth = 10;
const int m_roomHeight = 20;
int Level[m_roomHeight][m_roomWidth] = {0};
struct Tetra {int PosX, PosY;}TetraA[4],TetraB[4];
// defino piezas
int Piezas[7][4] =
        {
           1,3,5,7, // I
           2,4,5,7, // Z
           3,5,4,6, // S
           3,5,4,7, // T
           2,3,5,7, // L
           3,5,7,6, // J
           2,3,4,5, // O
        };
bool CheckCollision()
{
    for(int i=0;i<4;i++)
    {
        if(TetraA[i].PosX <0||TetraA[i].PosX >=m_roomWidth || TetraA[i].PosY >= m_roomHeight)
        {
            return 0;
        }
        else if (Level[TetraA[i].PosY] [TetraA[i].PosX])
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    srand(time(nullptr));
    sf::RenderWindow window(sf::VideoMode(320, 480), "Tetris!");
    sf::Texture m_tetraTex;
    sf::Texture m_frameTex;
    m_tetraTex.loadFromFile("../Images/tetris.png");
    m_frameTex.loadFromFile("../Images/tetris-frame.png");
    sf::Sprite m_tetraSpr(m_tetraTex);
    sf::Sprite m_frameSpr(m_frameTex);
    m_tetraSpr.setTextureRect(sf::IntRect(0, 0, 18, 18));
    int PiezaPosX = 0;
    int ColorNum = 1;
    bool bRotate = false;
    float timer = 0.0f;
    float delay = 0.3f;
    sf::Clock m_clock;

    while (window.isOpen())
    {
        // timer
        float time = m_clock.getElapsedTime().asSeconds();
        m_clock.restart();
        timer += time;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up) { bRotate = true; }
                if (event.key.code == sf::Keyboard::Left) { PiezaPosX = -1; }
                if (event.key.code == sf::Keyboard::Right) { PiezaPosX = 1; }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {delay = 0.05f;}
        }
        //moving
        for(int i=0;i<4;i++)
        {
            TetraB[i] = TetraA[i];
            TetraA[i].PosX += PiezaPosX;
        }
        //Collsion
        if(!CheckCollision())
        {
            for(int i=0;i<4;i++)
            {
                TetraA[i] = TetraB[i];
            }
        }

        if(bRotate)
        {
          Tetra pivot = TetraA[1];
          for (int i = 0; i < 4; i++)
          {
             int tempX = TetraA[i].PosY - pivot.PosY;
             int tempY = TetraA[i].PosX - pivot.PosX;
             TetraA[i].PosX = pivot.PosX - tempX;
             TetraA[i].PosY = pivot.PosY + tempY;
          }
          if(!CheckCollision())
          {
              for(int i=0;i<4;i++)
              {
                  TetraA[i]=TetraB[i];
              }
          }
        }
         // TICK
         if(timer>delay)
         {
             for(int i=0;i<4;i++)
             {
                 TetraB[i]=TetraA[i];
                 TetraA[i].PosY +=1;
             }
             if(!CheckCollision()) {
                 for (int i = 0; i < 4; i++)
                 {
                     Level[TetraB[i].PosY][TetraB[i].PosX] = ColorNum;
                 }
                 int n = rand() % 7;
                 ColorNum = 1 + n;
                 for (int i = 0; i < 4; i++)
                 {
                    TetraA[i].PosX = Piezas[n][i] % 2;
                    TetraA[i].PosY = Piezas[n][i] / 2;
                 }
             }
             timer = 0;
         }
         // Check Lines
         int k = m_roomHeight -1;
         for(int y= m_roomHeight-1;y>0;y--)
         {
             int count =0;
             for(int x = 0;x<m_roomWidth;x++)
             {
                 if((Level[y][x]))
                 {
                     count++;
                 }
                 Level[k][x] = Level[y][x];
             }
             if(count < m_roomWidth)
             {
                 k--;
             }
         }
         //Reset position and rotation
         PiezaPosX =0;
         bRotate= false;
         delay =0.3f;

         // DRAW
         window.clear(sf::Color::Black);
         for(int y=0;y < m_roomHeight;y++)
         {
             for(int x=0;x<m_roomWidth;x++)
             {
                 if(Level[y][x]==0)
                 {
                     continue;
                 }
                 // dibuja la pieza
                 m_tetraSpr.setTextureRect(sf::IntRect(Level[y][x] * 18, 0, 18, 18));
                 m_tetraSpr.setPosition(x * 18.0f, y * 18.0f);
                 m_tetraSpr.move(28, 31); // Offset
                 window.draw(m_tetraSpr);
             }
         }

        // se mueve un poquito a cada tick.
        for(int i=0;i<4;i++)
        {
            m_tetraSpr.setTextureRect(sf::IntRect(ColorNum * 18, 0, 18, 18));
            m_tetraSpr.setPosition(TetraA[i].PosX * 18.0f, TetraA[i].PosY * 18.0f);
            m_tetraSpr.move(28, 31);
            window.draw(m_tetraSpr);
        }
        window.draw(m_frameSpr);
        window.display();
    }
    return 0;
}
