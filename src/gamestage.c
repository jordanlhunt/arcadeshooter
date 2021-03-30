#include "gamestage.h"
// Decelerations
static int enemyCollisionPlayer(Entity *enemy);
static int laserCollisionShip(Entity *laser);
static void clipPlayer();
static void draw();
static void drawDebris();
static void drawExplosions();
static void drawHUD();
static void drawLasers();
static void drawShips();
static void drawPointPickUps();
static void fireAlienLaser();
static void fireLaser();
static void handleDebris();
static void handleEnemies();
static void handleExplosions();
static void handleLaser();
static void handlePlayer();
static void handlePointPickUps();
static void handleShips();
static void initPlayer();
static void logic();
static void resetStage();
static void spawnEnemies();
static void triggerDebris(Entity *entity);
static void triggerExplosions(int x, int y, int numberOfExplosions);
static void triggerPointPickUpSpawn(int x, int y);
// Instance Variables
static Entity *player;
static int enemySpawnTimer;
static int highscore;
static int stageResetTimer;
static SDL_Texture *enemyLaserTexture;
static SDL_Texture *enemyShipTexture;
static SDL_Texture *explosionTexture;
static SDL_Texture *bigPointsTexture;
static SDL_Texture *playerLaserTexture;
static SDL_Texture *playerShipTexture;
static SDL_Texture *smallPointsTexture;
// Setup the game stage
void initStage()
{
    // Set gamehandler's pointers to static functions
    app.gameManager.logic = logic;
    app.gameManager.draw = draw;
    memset(&gameStage, 0, sizeof(GameStage));
    // Add tails entities to their respective linked list
    gameStage.debrisTail = &gameStage.debrisHead;
    gameStage.explosionTail = &gameStage.explosionHead;
    gameStage.laserTail = &gameStage.laserHead;
    gameStage.shipTail = &gameStage.shipHead;
    gameStage.pointPickUpTail = &gameStage.pointPickUpHead;
    // Pre-cache images maximize memory efficiency
    enemyLaserTexture = loadTexture("images/alienLaser.png");
    enemyShipTexture = loadTexture("images/alienShip.png");
    explosionTexture = loadTexture("images/explosion.png");
    playerLaserTexture = loadTexture("images/playerLaser.png");
    playerShipTexture = loadTexture("images/playerShip.png");
    smallPointsTexture = loadTexture("images/smallPoint.png");
    bigPointsTexture = loadTexture("images/bigPoints.png");
    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
    resetStage();
    initPlayer();
    // Initialize Timer
    enemySpawnTimer = 0;
    gameStage.score = 0;
    stageResetTimer = FPS;
}
static void initPlayer()
{
    // Allocate Memory for player
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    // Add the player to linked list
    gameStage.shipTail->next = player;
    gameStage.shipTail = player;
    // Setup players start position
    player->hitPoints = 3;
    player->x = 100;
    player->y = 100;
    player->texture = playerShipTexture;
    // Set player w and h to width and height of texture
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
    player->side = SOURCE_PLAYER;
}

// Handles the GameLoop logic
static void logic()
{
    handleBackground();
    handleStarfield();
    handlePlayer();
    handleShips();
    handleEnemies();
    handleLaser();
    handlePointPickUps();
    spawnEnemies();
    clipPlayer();
    // If the player ship is defeated restart the game
    if (player == NULL && --stageResetTimer <= 0)
    {
        addHighScore(gameStage.score);
        initHighScores();
    }
    handleExplosions();
    handleDebris();
}
static void fireLaser()
{
    // Create an entity object
    Entity *laser;
    laser = malloc(sizeof(Entity));
    memset(laser, 0, sizeof(Entity));
    // Add the laser to the laser linked list;
    gameStage.laserTail->next = laser;
    gameStage.laserTail = laser;
    // Set the laser at the center of the player sprite
    laser->x = player->x;
    laser->y = player->y;
    // Move the laser across the screen
    laser->directionX = PLAYER_LASER_SPEED;
    laser->hitPoints = 1;
    // Set laser to pre-cached texture
    laser->texture = playerLaserTexture;
    // Set the side of the bullet to player
    laser->side = SOURCE_PLAYER;
    SDL_QueryTexture(laser->texture, NULL, NULL, &laser->w, &laser->h);
    laser->y += (player->h / 2) - (laser->h / 2);
    laser->x += (player->w) - laser->w;
    player->reload = 16;
}
static void fireAlienLaser(Entity *enemy)
{
    // Create the laser object
    Entity *laser;
    laser = malloc(sizeof(Entity));
    memset(laser, 0, sizeof(Entity));
    // Added to laser linked list
    gameStage.laserTail->next = laser;
    gameStage.laserTail = laser;
    // Origin
    laser->x = enemy->x;
    laser->y = enemy->y;
    laser->hitPoints = 1;
    laser->texture = enemyLaserTexture;
    laser->side = enemy->side;
    SDL_QueryTexture(laser->texture, NULL, NULL, &laser->w, &laser->h);
    laser->y += (enemy->h / 2) - (enemy->h / 2);
    laser->x += (enemy->w) - laser->w;
    calculateSlope(player->x + (player->w / 2), player->y + (player->h / 2), enemy->x, enemy->y, &laser->directionX, &laser->directionY);
    laser->directionX *= ENEMY_BULLET_SPEED;
    laser->directionY *= ENEMY_BULLET_SPEED;
    laser->side = SOURCE_ENEMY;
    enemy->reload = (rand() % FPS * 2);
}
static void handlePlayer()
{
    if (player != NULL)
    {
        player->directionX = 0;
        player->directionY = 0;
        // Reduce the timer on reload
        if (player->reload > 0)
        {
            player->reload--;
        }
        // Movement
        if (app.keyboard[SDL_SCANCODE_UP] || app.keyboard[SDL_SCANCODE_W])
        {
            player->directionY -= PLAYER_SPEED;
        }
        if (app.keyboard[SDL_SCANCODE_DOWN] || app.keyboard[SDL_SCANCODE_S])
        {
            player->directionY += PLAYER_SPEED;
        }
        if (app.keyboard[SDL_SCANCODE_LEFT] || app.keyboard[SDL_SCANCODE_A])
        {
            player->directionX -= PLAYER_SPEED;
        }
        if (app.keyboard[SDL_SCANCODE_RIGHT] || app.keyboard[SDL_SCANCODE_D])
        {
            player->directionX += PLAYER_SPEED;
        }
        // Shoot laser
        if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0 || app.keyboard[SDL_SCANCODE_J] && player->reload == 0)
        {
            playSound(SOUND_PLAYER_FIRE, CHANNEL_PLAYER);
            fireLaser();
        }
    }
}
static void handlePointPickUps()
{
    Entity *currentPointPickUp;
    Entity *previousPointPickUp;
    previousPointPickUp = &gameStage.pointPickUpHead;
    for (currentPointPickUp = gameStage.pointPickUpHead.next; currentPointPickUp != NULL; currentPointPickUp = currentPointPickUp->next)
    {
        if (currentPointPickUp < 0)
        {
            currentPointPickUp->x = 0;
            currentPointPickUp->directionX = -(currentPointPickUp->directionX);
        }
        if ((currentPointPickUp->x + currentPointPickUp->w) > SCREEN_WIDTH)
        {
            currentPointPickUp->x = SCREEN_WIDTH - currentPointPickUp->w;
            currentPointPickUp->directionX = -(currentPointPickUp->directionX);
        }
        if (currentPointPickUp->y < 0)
        {
            currentPointPickUp->y = 0;
            currentPointPickUp->directionY = -(currentPointPickUp->directionY);
        }
        if ((currentPointPickUp->y + currentPointPickUp->h) > SCREEN_HEIGHT)
        {
            currentPointPickUp->y = SCREEN_HEIGHT - currentPointPickUp->h;
            currentPointPickUp->directionY = -(currentPointPickUp->directionY);
        }
        currentPointPickUp->x += currentPointPickUp->directionX;
        currentPointPickUp->y += currentPointPickUp->directionY;
        if (player != NULL && collision(currentPointPickUp->x, currentPointPickUp->y, currentPointPickUp->w, currentPointPickUp->h, player->x, player->y, player->w, player->h))
        {
            if (currentPointPickUp->texture == bigPointsTexture)
            {
                gameStage.score += 5;
            }
            else
            {
                gameStage.score++;
            }
            currentPointPickUp->hitPoints = 0;
            highscore = MAX(gameStage.score, highscore);
            playSound(SOUND_POINT_PICK_UP, CHANNEL_POINT_PICK_UP);
        }
        if (currentPointPickUp->hitPoints == 0)
        {
            if (currentPointPickUp == gameStage.pointPickUpTail)
            {
                gameStage.pointPickUpTail = previousPointPickUp;
            }
            previousPointPickUp->next = currentPointPickUp->next;
            free(currentPointPickUp);
            currentPointPickUp = previousPointPickUp;
        }
        previousPointPickUp = currentPointPickUp;
    }
}
static void handleShips()
{
    Entity *ship;
    Entity *previous = &gameStage.shipHead;
    for (ship = gameStage.shipHead.next; ship != NULL; ship = ship->next)
    {
        // Add the direction's to the enemy x and y
        ship->x += ship->directionX;
        ship->y += ship->directionY;
        // Delete the ship if it goes off screen collides with player
        if (ship != player && (ship->x < -ship->w || enemyCollisionPlayer(ship)))
        {
            ship->hitPoints = 0;
        }
        if (ship->hitPoints == 0)
        {
            if (ship == player)
            {
                player = NULL;
            }
            if (ship == gameStage.shipTail)
            {
                gameStage.shipTail = previous;
            }
            previous->next = ship->next;
            free(ship);
            ship = previous;
        }
        previous = ship;
    }
}
static void handleLaser()
{
    Entity *laser;
    Entity *previousLaser = &gameStage.laserHead;
    // Loop through linked list of lasers
    for (laser = gameStage.laserHead.next; laser != NULL; laser = laser->next)
    {
        // Move the laser
        laser->x += laser->directionX;
        laser->y += laser->directionY;
        // If the laser reaches outside the bounds of the screen delete it or hits a target
        if (laserCollisionShip(laser) || laser->x > SCREEN_WIDTH || laser->y > SCREEN_HEIGHT || laser->x < -laser->w || laser->y < -laser->h)
        {
            if (laser == gameStage.laserTail)
            {
                gameStage.laserTail = previousLaser;
            }
            previousLaser->next = laser->next;
            free(laser);
            laser = previousLaser;
        }
        previousLaser = laser;
    }
}
static void handleEnemies()
{
    Entity *enemy;
    // Step through each enemy and fire a laser if possible
    for (enemy = gameStage.shipHead.next; enemy != NULL; enemy = enemy->next)
    {
        if (enemy != player)
        {
            enemy->y = MIN((MAX(enemy->y, 0)), (SCREEN_HEIGHT - enemy->h));
            if (player != NULL && enemy->reload-- <= 0)
            {
                fireAlienLaser(enemy);
                playSound(SOUND_ALIEN_FIRE, CHANNEL_ALIEN_FIRE);
            }
        }
    }
}

// Move each explosion based on their direction X and Y. Decrement alpha over to make it less visable over time.
static void handleExplosions()
{
    Explosion *current;
    Explosion *previous;
    previous = &gameStage.explosionHead;
    for (current = gameStage.explosionHead.next; current != NULL; current = current->next)
    {
        current->x += current->directionX;
        current->y += current->directionY;
        if (current->alpha-- <= 0)
        {
            if (current == gameStage.explosionTail)
            {
                gameStage.explosionTail = previous;
            }
            previous->next = current->next;
            free(current);
            current = previous;
        }
        previous = current;
    }
}
// Decrement the debris' life after the initial explosion
static void handleDebris()
{
    Debris *current;
    Debris *previous;
    previous = &gameStage.debrisHead;
    for (current = gameStage.debrisHead.next; current != NULL; current = current->next)
    {
        current->x += current->directionX;
        current->y += current->directionY;
        current->directionY += 0.5;
        if (current->lifespan-- <= 0)
        {
            if (current == gameStage.debrisTail)
            {
                gameStage.debrisTail = previous;
            }
            previous->next = current->next;
            free(current);
            current = previous;
        }
        previous = current;
    }
}
static int laserCollisionShip(Entity *laser)
{
    Entity *ship;
    int generatePointPickUp;
    for (ship = gameStage.shipHead.next; ship != NULL; ship = ship->next)
    {
        if (ship->side != laser->side && collision(laser->x, laser->y, laser->w, laser->h, ship->x, ship->y, ship->w, ship->h))
        {
            laser->hitPoints = 0;
            ship->hitPoints--;
            generatePointPickUp = rand() % 2;
            // Check if collision with player or alien
            if (ship == player)
            {
                playSound(SOUND_PLAYER_IS_HIT, CHANNEL_PLAYER);
            }
            else
            {
                playSound(SOUND_ALIEN_IS_HIT, CHANNEL_ANY);
            }
            // If the laser collision reduces HP to zero
            if (ship->hitPoints <= 0)
            {
                // Trigger sound for player death
                if (ship == player)
                {
                    playSound(SOUND_PLAYER_DIE, CHANNEL_PLAYER);
                    triggerExplosions(ship->x, ship->y, 32);
                    triggerDebris(ship);
                }
                // trigger sound for alien death
                else
                {
                    playSound(SOUND_ALIEN_DIE, CHANNEL_ANY);
                    triggerExplosions(ship->x, ship->y, 32);
                    triggerDebris(ship);
                }
                // Spawn pointPickUp if even number
                if (generatePointPickUp % 2 == 0)
                {
                    triggerPointPickUpSpawn(ship->x + (ship->w / 2), ship->y + (ship->h / 2));
                }
            }
            return 1;
        }
    }
    return 0;
}
static int enemyCollisionPlayer(Entity *enemy)
{
    Entity *ship;
    for (ship = gameStage.shipHead.next; ship != NULL; ship = ship->next)
    {
        if (ship == player && enemy->side != player->side && collision(enemy->x, enemy->y, enemy->w, enemy->h, ship->x, ship->y, ship->w, ship->h))
        {
            enemy->hitPoints = 0;
            ship->hitPoints--;
            return 1;
        }
    }
    return 0;
}
static void spawnEnemies()
{
    Entity *enemy;
    if (enemySpawnTimer-- <= 0)
    {
        // Create a enemy object
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        // Add to the linked list
        gameStage.shipTail->next = enemy;
        gameStage.shipTail = enemy;
        // Place the enemy on the screen
        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyShipTexture;
        enemy->side = SOURCE_ENEMY;
        enemy->hitPoints = 2;
        // Delay fire speed of enemy
        enemy->reload = FPS * (1 + (rand() % 5));
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
        // Move the ship to the left
        enemy->directionX = -(2 + (rand() % 4));
        enemy->directionY = -100 + (rand() % 200);
        enemy->directionY = enemy->directionY / 100;
        // Reset the timer
        enemySpawnTimer = 30 + (rand() % 60);
    }
}
static void draw()
{
    drawBackground();
    drawStarfield();
    // Draw each game component
    drawShips();
    drawLasers();
    drawDebris();
    drawExplosions();
    drawPointPickUps();
    drawHUD();
}

static void drawLasers()
{
    // Create a laser pointer
    Entity *laser;
    for (laser = gameStage.laserHead.next; laser != NULL; laser = laser->next)
    {
        blit(laser->texture, laser->x, laser->y);
    }
}
static void drawShips()
{
    Entity *ship;
    for (ship = gameStage.shipHead.next; ship != NULL; ship = ship->next)
    {
        blit(ship->texture, ship->x, ship->y);
    }
}
// Loop through the debris list an draw the
static void drawDebris()
{
    Debris *debris;
    for (debris = gameStage.debrisHead.next; debris != NULL; debris = debris->next)
    {
        blitRectangle(debris->texture, &debris->rectangle, debris->x, debris->y);
    }
}
// Draw the Heads-up display
static void drawHUD()
{
    drawText(10, 10, 255, 255, 255, TEXT_LEFT, "SCORE: %03d", gameStage.score);

    if (gameStage.score < highScores.highScoresArray[0].score)
    {
        drawText(SCREEN_WIDTH - 10, 10, 255, 255, 255, TEXT_RIGHT, "HIGHSCORE: %03d", highScores.highScoresArray[0].score);
    }
    else
    {
        drawText(SCREEN_WIDTH - 10, 10, 0, 255, 0, TEXT_RIGHT, "HIGHSCORE: %03d", gameStage.score);
    }
}
//
static void drawExplosions()
{
    Explosion *explosion;
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);
    for (explosion = gameStage.explosionHead.next; explosion != NULL; explosion = explosion->next)
    {
        SDL_SetTextureColorMod(explosionTexture, explosion->red, explosion->green, explosion->blue);
        SDL_SetTextureAlphaMod(explosionTexture, explosion->alpha);
        blit(explosionTexture, explosion->x, explosion->y);
    }
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

static void drawPointPickUps()
{
    Entity *pointPickUp;
    for (pointPickUp = gameStage.pointPickUpHead.next; pointPickUp != NULL; pointPickUp = pointPickUp->next)
    {
        blit(pointPickUp->texture, pointPickUp->x, pointPickUp->y);
    }
}
static void resetStage()
{
    Explosion *explosion;
    Debris *debris;
    Entity *entity;
    // Delete existing ships
    while (gameStage.shipHead.next)
    {
        entity = gameStage.shipHead.next;
        gameStage.shipHead.next = entity->next;
        free(entity);
    }
    // Delete existing lasers
    while (gameStage.laserHead.next)
    {
        entity = gameStage.laserHead.next;
        gameStage.laserHead.next = entity->next;
        free(entity);
    }
    // Delete existing explosions
    while (gameStage.explosionHead.next)
    {
        explosion = gameStage.explosionHead.next;
        gameStage.explosionHead.next = explosion->next;
        free(explosion);
    }
    // Delete existing debris
    while (gameStage.debrisHead.next)
    {
        debris = gameStage.debrisHead.next;
        gameStage.debrisHead.next = debris->next;
        free(debris);
    }
    // Delete pickUps
    while (gameStage.pointPickUpHead.next)
    {
        entity = gameStage.pointPickUpHead.next;
        gameStage.pointPickUpHead.next = entity->next;
        free(entity);
    }
    memset(&gameStage, 0, sizeof(GameStage));
    gameStage.shipTail = &gameStage.shipHead;
    gameStage.laserTail = &gameStage.laserHead;
    gameStage.explosionTail = &gameStage.explosionHead;
    gameStage.debrisTail = &gameStage.debrisHead;
    gameStage.pointPickUpTail = &gameStage.pointPickUpHead;
}
static void clipPlayer()
{
    if (player != NULL)
    {
        if (player->x < 0)
        {
            player->x = 0;
        }
        if (player->y < 0)
        {
            player->y = 0;
        }
        if (player->x > SCREEN_WIDTH / 2)
        {
            player->x = SCREEN_WIDTH / 2;
        }
        if (player->y > SCREEN_HEIGHT - player->h)
        {
            player->y = SCREEN_HEIGHT - player->h;
        }
    }
}
static void triggerExplosions(int x, int y, int numberOfExplosions)
{
    Explosion *explosion;
    int i;
    for (i = 0; i < numberOfExplosions; i++)
    {
        // Create the explosion, allocate memory
        explosion = malloc(sizeof(Explosion));
        memset(explosion, 0, sizeof(Explosion));
        // Add the explosion to the linked list
        gameStage.explosionTail->next = explosion;
        gameStage.explosionTail = explosion;
        // Set the explosion at the location passed into the function
        explosion->x = x + (rand() % 32) - (rand() % 32);
        explosion->y = y + (rand() % 32) - (rand() % 32);
        explosion->directionX /= 10;
        explosion->directionY /= 10;
        // Assign a random color to the  explosion
        switch (rand() % 4)
        {
        case 0:
            explosion->red = 255;
            break;
        case 1:
            explosion->red = 255;
            explosion->green = 128;
            break;
        case 2:
            explosion->red = 255;
            explosion->green = 128;
            break;
        default:
            explosion->red = 255;
            explosion->green = 0;
            explosion->blue = 0;
            break;
        }
        // How long it will live for
        explosion->alpha = FPS;
    }
}
// Cut an entity into four parts for debris
static void triggerDebris(Entity *entity)
{
    Debris *debris;
    int x;
    int y;
    int width = entity->w / 2;
    int height = entity->h / 2;
    for (y = 0; y <= height; y += height)
    {
        for (x = 0; x <= width; x += width)
        {
            debris = malloc(sizeof(Debris));
            memset(debris, 0, sizeof(Debris));
            gameStage.debrisTail->next = debris;
            gameStage.debrisTail = debris;
            debris->x = entity->x + entity->w / 2;
            debris->y = entity->y + entity->h / 2;
            debris->directionX = (rand() % 5) - (rand() % 5);
            debris->directionY = -((rand() % 12) + 5);
            debris->lifespan = FPS * 2;
            debris->texture = entity->texture;
            debris->rectangle.x = x;
            debris->rectangle.y = y;
            debris->rectangle.w = width;
            debris->rectangle.h = height;
        }
    }
}
// Spawn a point pick for player
static void triggerPointPickUpSpawn(int x, int y)
{
    Entity *pointPickUp;
    int bigPointSpawn = (rand() % 7) + 1;
    // Create an object
    pointPickUp = malloc(sizeof(Entity));
    memset(pointPickUp, 0, sizeof(Entity));
    gameStage.pointPickUpTail->next = pointPickUp;
    gameStage.pointPickUpTail = pointPickUp;
    pointPickUp->x = x;
    pointPickUp->y = y;
    // Have the points have random spawn trajectory
    pointPickUp->directionX = -(rand() % 5);
    pointPickUp->directionY = (rand() % 5) - (rand() % 5);
    pointPickUp->hitPoints = 1;
    if (bigPointSpawn == 7)
    {
        pointPickUp->texture = bigPointsTexture;
    }
    else
    {
        pointPickUp->texture = smallPointsTexture;
    }
    SDL_QueryTexture(pointPickUp->texture, NULL, NULL, &pointPickUp->w, &pointPickUp->h);
    pointPickUp->x -= pointPickUp->w / 2;
    pointPickUp->y -= pointPickUp->y / 2;
}
