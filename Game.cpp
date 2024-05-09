/// 22i-1222 shoaib irfan 22i-1104 Mubariz haroon  22i-0996
#include "KrusKal.cpp"
#include <random>
#include <vector>
#include "avltreasure.cpp"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Line
{
public:
    RectangleShape shape;

    Line(float x, float y, float width, float height, sf::Color color = Color::Black)
    {
        shape.setSize(Vector2f(width, height));
        shape.setPosition(x, y);
        shape.setFillColor(color);
    }
};
int generateRandomDirection()
{
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate random number between 1 and 4
    std::uniform_int_distribution<> dis(1, 4);

    return dis(gen);
}
class Game
{
public:
    Node *shoaib;
    Node *ninventary;

    int rows, cols;
    vector<vector<Cell>> grid;
    // Initalize Maze Using Kruskal Algo
    void initializeMaze(int rows, int cols)
    {
        grid = initialize_grid(rows, cols);
        initialize_sets(grid);
        auto walls = get_all_walls(grid);

        shuffle_walls(walls);

        for (const auto &wall : walls)
        {
            Cell *cell1 = const_cast<Cell *>(wall.cell1);
            Cell *cell2 = const_cast<Cell *>(wall.cell2);
            if (find_set_id(cell1) != find_set_id(cell2))
            {
                remove_wall_between(cell1, cell2);
                merge_sets(cell1, cell2, grid);
            }
        }
        grid[rows - 1][0].left_wall = false;
        grid[0][cols - 1].right_wall = false;
    }
    void MovePlayer(int &a, int &b, int choice)
    {
        Cell cell = grid[a][b];

        if (choice == 1 && !cell.left_wall && b > 0)
        {
            b--;
        }
        else if (choice == 2 && !cell.right_wall && b < cols - 1)
        {
            b++;
        }
        else if (choice == 3 && !cell.top_wall && a > 0)
        {
            a--;
        }
        else if (choice == 4 && !cell.bottom_wall && a < rows - 1)
        {
            a++;
        }
    }

    void TomPlayer(int &a, int &b, int &prevChoice)
    {
        Cell cell = grid[a][b];
        int check = 0;
        int count = 0;
        // Check if the previous move is valid

        if (!cell.left_wall)
            count++;
        else if (!cell.right_wall)
            count++;
        if (!cell.top_wall)
            count++;
        else if (!cell.bottom_wall)
            count++;
        if (count < 2)
            if (prevChoice == 1 && !cell.left_wall && b > 0)
            {
                b--;
                check = 1;
            }
            else if (prevChoice == 2 && !cell.right_wall && b < cols - 1)
            {
                b++;
                check = 1;
            }
            else if (prevChoice == 3 && !cell.top_wall && a > 0)
            {
                a--;
                check = 1;
            }
            else if (prevChoice == 4 && !cell.bottom_wall && a < rows - 1)
            {
                a++;
                check = 1;
            }

        if (count >= 2)
        {
            if (prevChoice == 1)
            {
                if (prevChoice != 3 && !cell.top_wall && a > 0)
                {
                    a--;
                    prevChoice = 3;
                    check = 1;
                }
                else if (prevChoice != 4 && !cell.bottom_wall && a < rows - 1)
                {
                    a++;
                    prevChoice = 4;
                    check = 1;
                }
                else if (prevChoice != 2 && !cell.right_wall && b < cols - 1)
                {
                    b++;
                    prevChoice = 2;
                    check = 1;
                }
            }
            else if (prevChoice == 2)
            {
                if (prevChoice != 3 && !cell.top_wall && a > 0)
                {
                    a--;
                    prevChoice = 3;
                    check = 1;
                }
                else if (prevChoice != 4 && !cell.bottom_wall && a < rows - 1)
                {
                    a++;
                    prevChoice = 4;
                    check = 1;
                }
                else if (prevChoice != 1 && !cell.left_wall && b > 0)
                {
                    b--;
                    prevChoice = 1;
                    check = 1;
                }
            }
            else if (prevChoice == 3)
            {
                if (prevChoice != 1 && !cell.left_wall && b > 0)
                {
                    b--;
                    prevChoice = 1;
                    check = 1;
                }
                else if (prevChoice != 2 && !cell.right_wall && b < cols - 1)
                {
                    b++;
                    prevChoice = 2;
                    check = 1;
                }
                else if (prevChoice != 4 && !cell.bottom_wall && a < rows - 1)
                {
                    a++;
                    prevChoice = 4;
                    check = 1;
                }
            }
            else if (prevChoice == 4)
            {
                if (prevChoice != 1 && !cell.left_wall && b > 0)
                {
                    b--;
                    prevChoice = 1;
                    check = 1;
                }
                else if (prevChoice != 2 && !cell.right_wall && b < cols - 1)
                {
                    b++;
                    prevChoice = 2;
                    check = 1;
                }
                else if (prevChoice != 3 && !cell.top_wall && a > 0)
                {
                    a--;
                    prevChoice = 3;
                    check = 1;
                }
            }
        }
        if (check == 0)
        {

            if (prevChoice != 1 && !cell.left_wall && b > 0)
            {
                b--;
                prevChoice = 1;
            }
            else if (prevChoice != 2 && !cell.right_wall && b < cols - 1)
            {
                b++;
                prevChoice = 2;
            }
            else if (prevChoice != 3 && !cell.top_wall && a > 0)
            {
                a--;
                prevChoice = 3;
            }
            else if (prevChoice != 4 && !cell.bottom_wall && a < rows - 1)
            {
                a++;
                prevChoice = 4;
            }
        }
    }

    // Function to calculate the starting position of the maze to center it within the window
    pair<float, float> calculateMazeStartPosition(float windowWidth, float windowHeight, int rows, int cols, float cellSize, float borderWidth, float margin)
    {
        float mazeWidth = cols * cellSize + (cols + 1) * borderWidth;
        float mazeHeight = rows * cellSize + (rows + 1) * borderWidth;

        float mazeStartX = (windowWidth - mazeWidth) / 2.f;
        float mazeStartY = (windowHeight - mazeHeight) / 2.f;

        return {mazeStartX, mazeStartY};
    }
    void calculateMazeStartPosition(float windowWidth, float windowHeight, int rows, int cols, float cellSize, float borderWidth, float margin, float &a, float &b)
    {
        float mazeWidth = cols * cellSize + (cols + 1) * borderWidth;
        float mazeHeight = rows * cellSize + (rows + 1) * borderWidth;

        a = (windowWidth - mazeWidth) / 2.f;
        b = (windowHeight - mazeHeight) / 2.f;
    }

public:
    Game(int rows = 0, int cols = 0) : rows(rows), cols(cols) {}
    void displayImages(sf::RenderWindow &window, float mazeStartX, float mazeStartY, float cellSize)
    {
        // Load the first image (img2.png)
        sf::Texture texture;
        if (!texture.loadFromFile("cheese.png"))
        {

            // Error handling if the image fails to load
            std::cerr << "Failed to load image img2.png!" << std::endl;
            return;
        }

        // Create a sprite for the loaded texture

        sf::Sprite sprite(texture);
        // sprite.setScale(0.02,0.02);

        // Get the size of the window

        sf::Vector2u windowSize = window.getSize();

        // Set the position of the sprite to the upper right corner of the maze
        float spriteX = mazeStartX + cellSize * (cols - 1) - sprite.getGlobalBounds().width;
        float spriteY = mazeStartY;

        sprite.setPosition(spriteX + 170, spriteY + 70); // Adjusted position for img2.png

        // Draw the sprite onto the window
        window.draw(sprite);
    }

    void displayMaze(sf::RenderWindow &window, float mazeStartX, float mazeStartY, float cellSize, int px, int py)
    {
        // Define a new color using the RGB values corresponding to #fe8c14
        sf::Color wallColor(254, 140, 20); // R: 254, G: 140, B: 20
                                           ///   Color centerColor = sf::Color::Red; // Color for the center box

        mazeStartY += 100;

        for (size_t i = 0; i < grid.size(); ++i)
        {
            for (size_t j = 0; j < grid[0].size(); ++j)
            {
                float x = mazeStartX + j * cellSize;
                float y = mazeStartY + i * cellSize;

                // Draw walls
                if (grid[i][j].top_wall)
                    window.draw(Line(x, y, cellSize, 4.f, wallColor).shape);
                if (grid[i][j].bottom_wall)
                    window.draw(Line(x, y + cellSize, cellSize, 4.f, wallColor).shape);
                if (grid[i][j].left_wall)
                    window.draw(Line(x, y, 4.f, cellSize, wallColor).shape);
                if (grid[i][j].right_wall)
                    window.draw(Line(x + cellSize, y, 4.f, cellSize, wallColor).shape);
            }
        }
    }

    void run(float windowWidth, float windowHeight, int rows, int cols, float cellSize, float borderWidth, float margin)
    {

        sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight + 100), "SFML Maze");
        window.setFramerateLimit(60);
        sf::Vector2u windowSize = window.getSize();

        // Calculate scaleX and scaleY
        float scaleX = static_cast<float>(windowSize.x);
        float scaleY = static_cast<float>(windowSize.y + 100); // Add 100 pixels to the height

        // Load the background image
        sf::Texture texture2;
        texture2.loadFromFile("background.png");
        sf::Sprite sprite2(texture2);
        sprite2.setScale(scaleX / texture2.getSize().x, scaleY / texture2.getSize().y);
        int px = 0, py = rows - 1; // initial position player
        float mazeStartX = 0, mazeStartY = 0;
        calculateMazeStartPosition(windowWidth, windowHeight, rows, cols, cellSize, borderWidth, margin, mazeStartX, mazeStartY);
        //------------------------------------------initalize of all type of variable used in diffrent places
        srand(time(0));
        int a = 0;
        int score = 0;
        int inposition = 200;
        Clock clock, tomclock, shortestclock, startclock, endclock;
        int lockcheck = 0;
        float timer = 0;
        float tomtimer = 0;
        float shortesttimer = 5;
        float starttimer = 0, endtimer = 0;
        int speedc = 0;
        int firstmovecheck = 0;
        //-------------------------------------------initilize diffrent fonts variable
        Text text, leveldis;
        Font font;
        font.loadFromFile("Arial.ttf");
        //-------------------------------------------variable of image display
        sf::Texture mtexture;
        mtexture.loadFromFile("jerry.png");
        sf::Sprite msprite(mtexture);
        msprite.setScale(1, 1);

        //------------------------------------------varible of tom display

        sf::Texture tomtexture;
        tomtexture.loadFromFile("Tom.png");
        sf::Sprite tomSprite(tomtexture);
        tomSprite.setScale(0.8, 0.8);

        sf::Texture startg;
        startg.loadFromFile("startg.png");
        sf::Sprite startgs(startg);
        startgs.setScale(scaleX / startg.getSize().x, (scaleY - 100) / startg.getSize().y);
        sf::Texture endg;
        endg.loadFromFile("end.png");
        sf::Sprite endgs(endg);
        endgs.setScale(scaleX / endg.getSize().x, (scaleY - 100) / endg.getSize().y);
        // Calculate the position of the top-left corner of grid[2][0]
        sf::Texture ttexture;
        ttexture.loadFromFile("treasure.png");
        sf::Sprite tsprite(ttexture);
        tsprite.setScale(.7, .7);

        // Load texture for the second treasure

        //-------------------------------------------sward image
        sf::Texture stexture;
        stexture.loadFromFile("sword.png");
        sf::Sprite swordSprite(stexture);
        swordSprite.setScale(.8, .8);

        // Load texture for key
        sf::Texture ktexture;
        ktexture.loadFromFile("key.png");
        // Create sprite for key
        sf::Sprite keySprite(ktexture);
        keySprite.setScale(.8, .8);

        // Load texture for speed
        sf::Texture spdtexture;
        spdtexture.loadFromFile("thunder.png");
        // Create sprite for speed
        sf::Sprite speedSprite(spdtexture);
        speedSprite.setScale(.8, .8);

        // Load texture for lock
        sf::Texture lockTexture;
        lockTexture.loadFromFile("lock.png");

        // Create sprite for lock
        sf::Sprite lockSprite(lockTexture);
        lockSprite.setScale(0.8, 0.8);

        //---------------------------------------------random initialize treasure
        //
        int slo, klo, sslo, llo, tlo, t2lo;
        int ct = rand() % 19;
        int dt = (rand() % 14) + 2;

        float tsprite2X = mazeStartX + ct * cellSize;   // 0th column of the grid
        float tsprite2Y = (mazeStartY + dt * cellSize); // 2nd row of the grid
        shoaib = nullptr;
        shoaib = insert(shoaib, (tsprite2X + ct) * tsprite2Y, 1);
        tlo = (tsprite2X + ct) * tsprite2Y;
        tsprite.setPosition(tsprite2X + 5, tsprite2Y + 5);

        int check = 0;
        int cs = rand() % 19;
        int ds = (rand() % 14) + 2;
        while (check == 0)
        {
            check = 1;
            if (cs == ct && ds == dt)
            {
                check = 0;
            }
        }
        check = 0;
        float swordX = mazeStartX + cs * cellSize; // Calculate X position for sword
        float swordY = mazeStartY + ds * cellSize; // Calculate Y position for sword
        ninventary = nullptr;
        ninventary = insert(ninventary, (swordX + cs) * swordY, 2); // Assuming type 2 for sword
        slo = (swordX + cs) * swordY;
        // Set position for sword sprite
        swordSprite.setPosition(swordX + 5, swordY + 5);

        // Repeat similar process for key and speed
        int ck = rand() % 19;
        int dk = (rand() % 14) + 2;
        while (check == 0)
        {
            check = 1;
            if ((ck == cs && dk == ds) || (ck == ct && dk == dt))
            {
                check = 0;
            }
        }
        check = 0;
        float keyX = mazeStartX + ck * cellSize;                // Calculate X position for key
        float keyY = mazeStartY + dk * cellSize;                // Calculate Y position for key
        ninventary = insert(ninventary, (keyX + ck) * keyY, 3); // Assuming type 3 for key
        klo = (keyX + ck) * keyY;
        // Set position for key sprite
        keySprite.setPosition(keyX + 5, keyY + 5);

        int css = rand() % 19;
        int dss = (rand() % 14) + 2;
        while (check == 0)
        {
            check = 1;
            if ((css == ck && dss == dk) || (css == cs && dss == ds) || (css == ct && dss == dt))
            {
                check = 0;
            }
        }
        check = 0;
        float speedX = mazeStartX + css * cellSize;                  // Calculate X position for speed
        float speedY = mazeStartY + dss * cellSize;                  // Calculate Y position for speed
        ninventary = insert(ninventary, (speedX + css) * speedY, 4); // Assuming type 4 for speed
        sslo = (speedX + css) * speedY;
        speedX += 5;
        speedY += 5;
        // Set position for speed sprite
        speedSprite.setPosition(speedX, speedY);

        int cl = rand() % 2 + 17;
        int dl = (rand() % 2) + 2;
        while (check == 0)
        {
            check = 1;
            if ((cl == ck && dl == dk) || (cl == cs && dl == ds) || (cl == css && dl == dss) || (cl == ct && dl == dt))
            {
                check = 0;
            }
        }
        check = 0;
        float lockX = mazeStartX + cl * cellSize; // Calculate X position for speed
        float lockY = mazeStartY + dl * cellSize; // Calculate Y position for speed
        lockSprite.setPosition(lockX + 5, lockY + 5);
        llo = (lockX + cl) * lockY;

        int ctom = rand() % 7 + 5;
        int dtom = (rand() % 9) + 2;
        while (check == 0)
        {
            check = 1;
            if ((ctom == ck && dtom == dk) || (ctom == cs && dtom == ds) || (ctom == css && dtom == dss) || (ctom == ct && dtom == dt) || (ctom == cl && dtom == dl))
            {
                check = 0;
            }
        }
        check = 0;
        float tomX = mazeStartX + ctom * cellSize; // Calculate X position for speed
        float tomY = mazeStartY + dtom * cellSize; // Calculate Y position for speed
        tomSprite.setPosition(tomX + 5, tomY + 5);
        tlo = (tomX + ctom) * tomY;

        int previousmove = 3;
        tomtimer = 1;

        int menus = 0;

        sf::Texture page1;
        page1.loadFromFile("play.png");
        sf::Sprite page1s(page1);
        page1s.setScale(scaleX / page1.getSize().x, (scaleY - 100) / page1.getSize().y);
        // Scale the sprite to fit the window size

        //  page1s.setScale(scaleX, scaleY);

        // Likewise for instruction, highscore, and exit
        sf::Texture instruction;
        instruction.loadFromFile("instruction.png");
        sf::Sprite instructions(instruction);
        instructions.setScale(scaleX / instruction.getSize().x, (scaleY - 100) / instruction.getSize().y);

        sf::Texture highscore;
        highscore.loadFromFile("highscore.png");
        sf::Sprite highscores(highscore);
        highscores.setScale(scaleX / highscore.getSize().x, (scaleY - 100) / highscore.getSize().y);
        sf::Texture exit;
        exit.loadFromFile("exit.png");
        sf::Sprite exits(exit);
        exits.setScale(scaleX / exit.getSize().x, (scaleY - 100) / exit.getSize().y);

        sf::Texture instructionin;
        instructionin.loadFromFile("instructionin.png");
        sf::Sprite instructionins(instructionin);
        instructionins.setScale(scaleX / instructionin.getSize().x, (scaleY - 100) / instructionin.getSize().y);
        sf::Texture highscorein;
        highscorein.loadFromFile("highscorein.png");
        sf::Sprite highscoreins(highscorein);
        highscoreins.setScale(scaleX / highscorein.getSize().x, (scaleY - 100) / highscorein.getSize().y);
        vector<pair<pair<int, int>, vector<pair<int, int>>>> mazeData = store_maze_data(grid);

        int shortes = 0;

        while (window.isOpen())
        {

            //  cout << menus << endl;
            if (menus == 0 || menus == 1)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    else if (event.key.code == Keyboard::Down)
                        menus++;

                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        menus = 40;
                        starttimer = 0;
                    }
                }
                window.draw(page1s);
                window.display();
            }

            // Likewise for instruction, highscore, and exit
            if (menus == 2 || menus == 3)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    if (event.key.code == Keyboard::Up) // this key is for rotation
                        menus--;
                    else if (event.key.code == Keyboard::Down)
                        menus++;
                    else if (event.key.code == sf::Keyboard::Enter)
                        menus = 20;
                }
                window.draw(instructions);
                window.display();
            }
            if (menus == 4 || menus == 5)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    if (event.key.code == Keyboard::Up) // this key is for rotation
                        menus--;
                    else if (event.key.code == Keyboard::Down)
                        menus++;
                    else if (event.key.code == sf::Keyboard::Enter)
                        menus = 30;
                }
                window.draw(highscores);
                window.display();
            }
            if (menus == 6)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    if (event.key.code == Keyboard::Up) // this key is for rotation
                        menus--;
                    else if (event.key.code == sf::Keyboard::Enter)
                        window.close();
                }
                window.draw(exits);
                window.display();
            }

            if (menus == 20)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.key.code == sf::Keyboard::E)
                    {
                        menus = 2;
                    }
                }
                window.draw(instructionins);
                window.display();
            }
            if (menus == 30)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.key.code == sf::Keyboard::E)
                    {
                        menus = 4;
                    }
                }
                window.draw(highscoreins);
                window.display();
            }
            if (menus == 40)
            {

                float starttime = shortestclock.getElapsedTime().asSeconds();
                shortestclock.restart();
                starttimer += starttime;
                window.draw(startgs);
                window.display();
                if (starttimer > 4)
                    menus = 10;
            }
            if (menus == 50)
            {

                float starttime = shortestclock.getElapsedTime().asSeconds();
                shortestclock.restart();
                starttimer += starttime;
                window.draw(endgs);
                window.display();
                if (starttimer > 3)
                    window.close();
            }

            if (menus == 10)
            {
                sf::Event event;
                shortes = 0;
                //------------------------------------------for the control of player
                int prex = px, prey = py;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    if (a == 0 || speedc == 1)
                        if (event.key.code == Keyboard::Up) // this key is for rotation
                            MovePlayer(py, px, 3);
                        else if (event.key.code == Keyboard::Left) // this key is for left move ment
                            MovePlayer(py, px, 1);
                        else if (event.key.code == Keyboard::Right) // this key is for right movement
                            MovePlayer(py, px, 2);
                        else if (event.key.code == Keyboard::Down)
                        {
                            MovePlayer(py, px, 4);
                        }
                        else if (event.key.code == sf::Keyboard::E)
                            window.close();

                        else if (event.key.code == sf::Keyboard::S)
                            shortesttimer = 0;
                    a++;
                    if (a >= 2)
                        a = 0;
                }

                // cout << px << py << endl;
                float time = clock.getElapsedTime().asSeconds();
                clock.restart();
                timer += time;
                float stime = shortestclock.getElapsedTime().asSeconds();
                shortestclock.restart();
                shortesttimer += stime;

                if (speedc == 1 && timer > 5)
                {
                    speedc = 0;
                }
                //-------------------------------------------------for the movenemt of player
                float sprite2X = mazeStartX + px * cellSize;       // 0th column of the grid
                float sprite2Y = mazeStartY + (py + 2) * cellSize; // 2nd row of the grid
                if (score < 0 || (px == 19 && py == 0))
                {
                    menus = 50;
                    starttimer = 0;
                }
                // Set the position of sprite2 to be at the top-left corner of grid[2][0]
                msprite.setPosition(sprite2X + 5, sprite2Y + 2);
                // sprite2Y = mazeStartY + py * cellSize; // 2nd row of the grid
                //--------------------------------------------------for check of interaction with treasure
                if ((sprite2X + px) * sprite2Y == llo)
                {
                    if (lockcheck == 1)
                    {
                        lockSprite.setPosition(-100, -100);
                    }
                    else
                    {
                        py = prey;
                        px = prex;
                        float sprite2X = mazeStartX + px * cellSize; // 0th column of the grid
                        float sprite2Y = mazeStartY + py * cellSize; // 2nd row of the grid
                        // Set the position of sprite2 to be at the top-left corner of grid[2][0]
                        msprite.setPosition(sprite2X + 5, sprite2Y + 2);
                    }
                }

                if (search(shoaib, ((sprite2X + px) * sprite2Y)))
                {

                    score += 10;
                    tsprite.setPosition(-100, -100);                         // Move treasure off-screen
                    shoaib = deleteNode(shoaib, (sprite2X + px) * sprite2Y); // Remove treasure from the list
                }

                int searchcheck = search(ninventary, (sprite2X + px) * sprite2Y);
                if (searchcheck)
                {
                    if (slo == (sprite2X + px) * sprite2Y)
                    {
                        score += 5;
                        swordSprite.setPosition(inposition, 20);                         // Move sword off-screen
                        ninventary = deleteNode(ninventary, (sprite2X + px) * sprite2Y); // Remove sword from the list
                    }
                    else if (klo == (sprite2X + px) * sprite2Y)
                    {
                        keySprite.setPosition(inposition, 20);                           // Move key off-screen
                        ninventary = deleteNode(ninventary, (sprite2X + px) * sprite2Y); // Remove key from the list
                        lockcheck = 1;
                    }
                    else if (sslo == (sprite2X + px) * sprite2Y)
                    {
                        speedSprite.setPosition(inposition, 20);                         // Move speed off-screen
                        ninventary = deleteNode(ninventary, (sprite2X + px) * sprite2Y); // Remove speed from the list
                        speedc = 1;
                        timer = 0;
                    }
                    inposition += 50;
                }

                float tomtime = tomclock.getElapsedTime().asSeconds();
                tomclock.restart();
                tomtimer += tomtime;
                //   cout << tomtimer<<endl;
                if (tomtimer > 0.5)
                {
                    TomPlayer(dtom, ctom, previousmove);
                    tomX = mazeStartX + ctom * cellSize;       // Calculate X position for speed
                    tomY = mazeStartY + (dtom + 2) * cellSize; // Calculate Y position for speed
                    tomSprite.setPosition(tomX + 5, tomY + 5);
                    tlo = (tomX + ctom) * tomY;
                    tomtimer = 0;
                    firstmovecheck = 1;
                }

                if ((sprite2X + px) * sprite2Y == tlo)
                {
                    score -= 5;
                    ctom = rand() % 7 + 5;
                    dtom = (rand() % 9) + 2;
                    while (check == 0)
                    {
                        check = 1;
                        if ((ctom == ck && dtom == dk) || (ctom == cs && dtom == ds) || (ctom == css && dtom == dss) || (ctom == ct && dtom == dt) || (ctom == cl && dtom == dl))
                        {
                            check = 0;
                        }
                    }
                    check = 0;
                    tomX = mazeStartX + ctom * cellSize;       // Calculate X position for speed
                    tomY = mazeStartY + (dtom + 2) * cellSize; // Calculate Y position for speed
                    tomSprite.setPosition(tomX + 5, tomY + 5);
                    tlo = (tomX + ctom) * tomY;
                    tomtimer = 1;
                }

                ///----------------------------------------------anytype of text and written thing initializer
                text.setFont(font);
                text.setString("Score: ");
                text.setPosition(850, 10);
                text.setCharacterSize(40);
                text.setFillColor(Color::White);
                Text dis_score(std::to_string(score), font);
                dis_score.setPosition(970, 10);
                dis_score.setCharacterSize(40);
                dis_score.setFillColor(Color::White);

                Text inventarytext;
                inventarytext.setFont(font);
                inventarytext.setString("Inventary: ");
                inventarytext.setPosition(20, 10);
                inventarytext.setCharacterSize(40);
                inventarytext.setFillColor(Color::White);

                //---------------------------------------------display all things
                window.draw(sprite2);
                displayImages(window, mazeStartX, mazeStartY, cellSize);
                displayMaze(window, mazeStartX, mazeStartY, cellSize, px, py);
                window.draw(msprite);
                window.draw(tsprite);
                window.draw(text);
                window.draw(inventarytext);
                window.draw(dis_score);

                if (shortesttimer <= 5)
                {
                    pair<int, int> start = {py, px};   // Player's position
                    pair<int, int> treasure = {0, 19}; // Treasure's position
                    vector<pair<int, int>> shortestPath = find_shortest_path(mazeData, start, treasure);
                    Color centerColor = sf::Color::Red; // Color for the center box
                    // Output the coordinates of the shortest path on the console
                    //   cout << "Shortest Path Coordinates: ";
                    int i = 0;
                    for (const auto &cell : shortestPath)
                    {
                        //    cout << "(" << cell.first << ", " << cell.second << ") ";
                        if (i == 0)
                            i = 1;
                        else
                        {
                            float x = mazeStartX + cell.second * cellSize;
                            float y = mazeStartY + (cell.first + 2) * cellSize;

                            float centerX = x + cellSize / 2;
                            float centerY = y + cellSize / 2;

                            // Draw red box at the center
                            sf::RectangleShape centerBox(sf::Vector2f(cellSize / 4, cellSize / 4));
                            centerBox.setFillColor(centerColor);
                            centerBox.setPosition(centerX - cellSize / 8, centerY - cellSize / 8); // Center the box

                            window.draw(centerBox);
                        }
                    }
                    cout << endl; // Print a newline after all coordinates are printed
                }

                window.draw(swordSprite);
                window.draw(keySprite);
                window.draw(speedSprite);
                window.draw(lockSprite);
                if (firstmovecheck == 1)
                    window.draw(tomSprite);
                window.display();
            }
        }
    }
};