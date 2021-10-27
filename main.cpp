/**
 * TETRIS ->
 * cosas que faltan:
 * UI (score, lines, level) -> lado derecho de la pantalla.
 * definir Level y Piezas por separado (ahora mismo es un bardo)
 * Definir game over , restart , una pantalla de titulo.
 */

#include <time.h>
#include <iostream>
#include <SFML/Graphics.hpp>
const int m_roomWidth = 10;
const int m_roomHeight = 20;
int m_lines = 0;
int m_bestLines = 2;
int Level[m_roomHeight][m_roomWidth] = {0};
struct TetraST {int PosX, PosY;};
TetraST TetraA[4];
TetraST TetraB[4];
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
    for(auto & tetra : TetraA)
    {
        if(tetra.PosX < 0 || tetra.PosX >= m_roomWidth || tetra.PosY >= m_roomHeight)
        {
            return false;
        }
        else if (Level[tetra.PosY] [tetra.PosX])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    srand(time(nullptr));
    sf::RenderWindow window(sf::VideoMode(320, 480), "Tetris!");
    sf::Texture m_tetraTex,m_frameTex;
    m_tetraTex.loadFromFile("../Images/tetris.png");
    m_frameTex.loadFromFile("../Images/tetris-frame.png");
    sf::Sprite m_tetraSpr(m_tetraTex),m_frameSpr(m_frameTex);
    //TODO: añadir font y text para la ui.
    int PiezaPosX = 0;
    int ColorNum = 1;
    m_lines = 0;
    bool bRotate = false;
    float timer = 0.0f;
    float delay = 0.5f;
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
        if(!CheckCollision())
        {
            for(int i=0;i<4;i++)
            {
                TetraA[i] = TetraB[i];
            }
        }

        if(bRotate)
        {
          TetraST pivot = TetraA[1];
          for (auto & tetra : TetraA)
          {
             int tempX = tetra.PosY - pivot.PosY;
             int tempY = tetra.PosX - pivot.PosX;
              tetra.PosX = pivot.PosX - tempX;
              tetra.PosY = pivot.PosY + tempY;
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
             if(!CheckCollision())
             {
                 for (auto & tetra : TetraB)
                 {
                     Level[tetra.PosY][tetra.PosX] = ColorNum;
                 }
                 int n = rand() % 7;
                 ColorNum = 1+ n;
                 for (int i = 0; i < 4; i++)
                 {
                    TetraA[i].PosX = Piezas[n][i] % 2;
                    TetraA[i].PosY = Piezas[n][i] / 2;
                 }
             }
             timer = 0;
         }
         // Check Lines
         int currentLine = m_roomHeight - 1;
         for(int y= m_roomHeight-1;y>0;y--)
         {
             int count =0;
             for(int x = 0;x<m_roomWidth;x++)
             {
                 if((Level[y][x]))
                 {
                     count++;
                 }
                 Level[currentLine][x] = Level[y][x];
             }
             // if count is less than the room for pieces.. means that the line is full --> delete the line.
             if(count < m_roomWidth)
             {
                 currentLine--;
                 /**
                  *                      // TODO : add score and lines.
                     m_lines++;
                     std::cout << "Lines :"<< m_lines << std::endl;
                     if(m_bestLines < m_lines)
                     {
                         m_bestLines = m_lines;
                         std::cout << "RECORD :"<< m_bestLines << std::endl;
                     }
                  */
             }
         }
         //Reset position and rotation
         PiezaPosX =0;
         bRotate= false;
         delay =0.5f;

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
        for(auto & i : TetraA)
        {
            m_tetraSpr.setTextureRect(sf::IntRect(ColorNum * 18, 0, 18, 18));
            m_tetraSpr.setPosition(i.PosX * 18, i.PosY * 18);
            m_tetraSpr.move(28, 31);
            window.draw(m_tetraSpr);
        }
        window.draw(m_frameSpr);
        window.display();
    }
    return 0;
}
