#include "raylib.h"
#include "raymath.h"
#include "Header.h"
#include <ctime>
#include <iostream>
#include <String>
#include <vector>
#include<algorithm>

using namespace std;//mutants

const int ATE = -60;
const int numTypes = 5;//intial number of each type of player
const int NOFOOD = INT_MAX;
const int screenWidth = 900;//og:800
const int screenHeight = 500;//og:450
int oldMutantSize = 0;

int indexOf(std::vector<int> foodRanking, int num) {
    for (int i = 0; i < foodRanking.size(); i++)
        if (num == foodRanking[i])
            return i;
    return -1;
}

int getRandWidth() { return rand() % (screenWidth - 20) + 20; }

int getRandHeight() { return rand() % (screenHeight - 20) + 20; }


std::vector<Vehicle> evolvePlayer(std::vector<Vehicle> players, int playersSize, std::vector<int> foodRanking, float eatIncrease, float detectIncrease) {
    for (int i = playersSize - 1; i >= 0; i--) {//evolving
        if (players.size() - 1 < i)
            continue;
        cout << "PlayersSize: " << playersSize << ", " << players.size() << ", " << i << " \n";
        Vehicle player = players[i];
        int foodCnt = player.foodCnt;
        int randNum = rand() % 10;
        cout << "ERROR 1\n";
        if (foodCnt == foodRanking[0]) {//erases last place(includes all ties)
            players.erase(players.begin() + i);
            cout << "ERASED " << i << "\n";
            playersSize--;
        }
        else if (foodCnt == foodRanking[foodRanking.size() - 1]) {//checks if 1st place
            //cout << "EVOLVING\n";
            players[i].resetAndIncreaseStats(Vector2({ (float)getRandWidth(), (float)getRandHeight() }), player.maxspeed * 0.5f, player.maxforce * 0.5f, eatIncrease + 5, detectIncrease + 5, 0.5);
        }
        else if (randNum >= 8 && playersSize > 2 && foodCnt == foodRanking[foodRanking.size() - 2]) {//2nd place
            players[i].resetAndIncreaseStats(Vector2({ (float)getRandWidth(), (float)getRandHeight() }), player.maxspeed * 0.45f, player.maxforce * 0.45f, eatIncrease * 0.75 + 5, detectIncrease * 0.9f + 5, 0.45);
        }
        else if (randNum >= 6 && playersSize > 3 && foodCnt == foodRanking[foodRanking.size() - 3]) {//3rd place
            players[i].resetAndIncreaseStats(Vector2({ (float)getRandWidth(), (float)getRandHeight() }), player.maxspeed * 0.4f, player.maxforce * 0.4f, eatIncrease * 0.5 + 5, detectIncrease * 0.8f + 5, 0.4);
        }
        else if (randNum >= 4 && playersSize > 4 && foodCnt == foodRanking[foodRanking.size() - 4]) {//4th place
            players[i].resetAndIncreaseStats(Vector2({ (float)getRandWidth(), (float)getRandHeight() }), player.maxspeed * 0.35f, player.maxforce * 0.35f, player.eatingRange * 0.25 + 5, detectIncrease * 0.75f + 5, 0.3);
        }
        else {
            players[i].resetAndIncreaseStats(Vector2({ (float)getRandWidth(), (float)getRandHeight() }), 0, 0, 0, 0, 0);//default reset
        }
        cout << "ERROR 2\n";
    }
    cout << "ERROR 3\n";
    return players;
}

std::vector<int> fillFoodRankings(std::vector<int> foodRanking, std::vector<Vehicle> typeOne, std::vector<Vehicle> typeTwo, std::vector<Vehicle> mutants) {
    for (int i = 0; i < typeOne.size(); i++)
        if (indexOf(foodRanking, typeOne[i].foodCnt) == -1)
            foodRanking.push_back(typeOne[i].foodCnt);

    for (int i = 0; i < typeTwo.size(); i++)
        if (indexOf(foodRanking, typeTwo[i].foodCnt) == -1)
            foodRanking.push_back(typeTwo[i].foodCnt);

    for (int i = 0; i < oldMutantSize; i++)
        if (indexOf(foodRanking, mutants[i].foodCnt) == -1)
            foodRanking.push_back(mutants[i].foodCnt);
    return foodRanking;
}

int main(void) {//MAIN
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(30);

    Image icon = LoadImage("flan.png");
    ImageResize(&icon, 50, 50);
    Texture2D textureType1 = LoadTextureFromImage(icon);
    icon = LoadImage("flanType2.png");
    ImageResize(&icon, 50, 50);
    Texture2D textureType2 = LoadTextureFromImage(icon);
    icon = LoadImage("flanMutant.png");
    ImageResize(&icon, 50, 50);
    Texture2D textureMutant = LoadTextureFromImage(icon);

    struct {
        int ballsX[70];
        int ballsY[70];
    }food;
    int numFood = sizeof(food.ballsX) / sizeof(food.ballsX[0]);

    std::vector<Vehicle> typeOne;
    std::vector<Vehicle> typeTwo;
    std::vector<Vehicle> mutants;

    for (int i = 0; i < numTypes; i++) {//intializes the type arrays with random values
        Vehicle temp1((float)getRandWidth(), (float)getRandHeight(), 2, 0.1f, 30, 0, 3);//speed: 2, force:0.1, eatingRange(Red circle): 30, detectionRange: 0, rechargeTime: 3
        typeOne.push_back(temp1);
        typeOne[i].invsTarget = Vector2({ (float)(getRandWidth()), (float)(getRandHeight()) });//wandering by chasing targets that are not drawn(invisible)

        Vehicle temp2((float)getRandWidth(), (float)getRandHeight(), 1, 0.1f, 5, 32, 0);//speed: 1, force:0.1, eatingRange: 5, detectionRange(Black line circle): 32, rechargeTime: 0
        typeTwo.push_back(temp2);
        typeTwo[i].invsTarget = Vector2({ (float)(getRandWidth()), (float)(getRandHeight()) });
    }
    /*for (int i = 0; i < numTypes; i++) {
        Vehicle temp((float)getRandWidth(), (float)getRandHeight(), 2, 0.1f, 20, 40, 3);//speed: 2, eatingRange(Red circle): 40, detectionRange: 0, rechargeTime: 3
        mutants.push_back(temp);
        mutants[i].invsTarget = Vector2({ (float)(getRandWidth()), (float)(getRandHeight()) });//wandering by chasing targets that are not drawn(invisible)
    }*/

    for (int i = 0; i < numFood; i++) {//intializes food positions
        food.ballsX[i] = getRandWidth();
        food.ballsY[i] = getRandHeight();
        //cout << "X: " << food.ballsX[i] << "\t Y:" << food.ballsY[i] << "\n";
    }

    float totalTime = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLUE);
        totalTime += GetFrameTime();

        if (totalTime >= 27 && (typeOne.size() > 0 || typeTwo.size() > 0 || mutants.size() > 0)) {//end of each round
            std::vector<int> foodRanking;
            oldMutantSize = mutants.size();//allows for newly spawn mutants not to be spawn killed
            foodRanking = fillFoodRankings(foodRanking, typeOne, typeTwo, mutants);
            std::sort(foodRanking.begin(), foodRanking.end());
            for (int i = 0; i < typeOne.size(); i++) {//searching through type 1s
                for (int j = 0; j < typeTwo.size(); j++) {//searching through type 2s
                    //cout << "FULKROM\n";
                    int distanceTo = (int)typeOne[i].getDistanceTo(typeTwo[j].location);
                    if (distanceTo <= 75 && typeOne[i].location.x != ATE && typeTwo[j].location.x != ATE) {//if within mutate distance
                        cout << "MUTANT\n";
                        Vehicle t1 = typeOne[i];
                        Vehicle t2 = typeTwo[j];
                        float newSpeed = t1.maxspeed > t2.maxspeed ? t1.maxspeed : t2.maxspeed;
                        float newForce = t1.maxforce > t2.maxforce ? t1.maxforce : t2.maxforce;//rand() % 2 == 1
                        /*if (randNum < 5)
                            mutants.push_back(Vehicle((float)getRandWidth(), (float)getRandHeight(), (t1.maxspeed + t2.maxspeed) / 2, (t1.maxforce + t2.maxforce) / 2, 5, t2.detectionRange * 0.8, 0));
                        else if(randNum >= 5 && randNum != 10)
                            mutants.push_back(Vehicle((float)getRandWidth(), (float)getRandHeight(), (t1.maxspeed + t2.maxspeed) / 2, (t1.maxforce + t2.maxforce) / 2, t1.eatingRange * 0.75, 0, t1.rechargeTime * 1.25));
                        else*/
                        mutants.push_back(Vehicle((t1.location.x + t2.location.x)/ 2, (t1.location.y + t2.location.y)/ 2, newSpeed, newForce, t1.eatingRange * 1.3, t2.detectionRange *0.75, t1.rechargeTime * 1.5));
                        mutants[mutants.size() - 1].reset(Vector2({ screenWidth, screenHeight }));
                        typeOne[i].location.x = ATE;
                        typeTwo[j].location.x = ATE;
                    }
                }
            }
            cout << "END MUTANTS\n";
            for (int i = typeOne.size() - 1; i >= 0; i--) {
                if (typeOne[i].location.x == ATE)
                    typeOne.erase(typeOne.begin() + i);
            }
            for (int i = typeTwo.size() - 1; i >= 0; i--) {
                if (typeTwo[i].location.x == ATE)
                    typeTwo.erase(typeTwo.begin() + i);
            }

            cout << "EVOLVING\n";//STATS FOR BALANCE CHANGES
            typeOne = evolvePlayer(typeOne, typeOne.size(), foodRanking, 4, 0);
            cout << "EVOLVED1\n";
            typeTwo = evolvePlayer(typeTwo, typeTwo.size(), foodRanking, 0, 14);
            cout << "EVOLVED2\n";
            mutants = evolvePlayer(mutants, oldMutantSize, foodRanking, 2, 7);
            cout << "EVOLVED3\n";

            for (int i = 0; i < numFood; i++) {//resets food positions
                food.ballsX[i] = getRandWidth();
                food.ballsY[i] = getRandHeight();
            }
            totalTime = 0;
        }
        //cout << "TYPE 1\n";
        for (int i = 0; i < typeOne.size(); i++) {//Type One player 
            //cout << totalTime << "\n";
            if (totalTime - typeOne[i].lastEaten >= typeOne[i].rechargeTime) {
                for (int j = 0; j < numFood; j++) {//checks if food is in range
                    if (food.ballsX[j] == ATE)
                        continue;
                    if (typeOne[i].getDistanceTo(Vector2({ (float)food.ballsX[j], (float)food.ballsY[j] })) <= typeOne[i].eatingRange) {
                        food.ballsX[j] = ATE;
                        food.ballsY[j] = ATE;
                        typeOne[i].foodCnt++;
                        typeOne[i].lastEaten = totalTime;
                        //cout << "TYPE 1 ATE\n";
                        break;
                    }
                }
            }
            if (typeOne[i].getDistanceTo(typeOne[i].invsTarget) <= 5) {//wandering targets
                typeOne[i].invsTarget = Vector2({ (float)(getRandWidth()), (float)(getRandHeight()) });
                //cout << "TYPE 1 CAUGHT IT\n";
            }
            typeOne[i].seek(typeOne[i].invsTarget);
            typeOne[i].update();
        }
        //cout << "TYPE 2\n";
        for (int i = 0; i < typeTwo.size(); i++) {//Type Two player
            int closestFoodDistance = NOFOOD;
            int closestFoodIndex = -1;
            for (int j = 0; j < numFood; j++) {//checks if food is in range
                if (food.ballsX[j] == ATE)
                    continue;
                int foodDistance = (int)typeTwo[i].getDistanceTo(Vector2({ (float)food.ballsX[j], (float)food.ballsY[j] }));
                if (typeTwo[i].invsTarget.x == ATE) {
                    typeTwo[i].invsTarget = Vector2({ (float)(getRandWidth()), (float)(getRandHeight()) });
                }
                if (foodDistance <= typeTwo[i].eatingRange) {
                    food.ballsX[j] = ATE;
                    food.ballsY[j] = ATE;
                    typeTwo[i].foodCnt++;
                    //cout << "TYPE 2 ATE\n";
                }
                if (foodDistance <= typeTwo[i].detectionRange && foodDistance < closestFoodDistance) {
                    closestFoodDistance = foodDistance;
                    closestFoodIndex = j;
                }
            }
            if (typeTwo[i].getDistanceTo(typeTwo[i].invsTarget) <= 5) {//wandering targets
                typeTwo[i].invsTarget = Vector2({ (float)(getRandWidth()), (float)(getRandHeight()) });
                // cout << "TYPE 2 CAUGHT IT\n";
            }
            if (closestFoodDistance != NOFOOD) {
                typeTwo[i].invsTarget = Vector2({ (float)(food.ballsX[closestFoodIndex]), (float)(food.ballsY[closestFoodIndex]) });
            }
            typeTwo[i].seek(typeTwo[i].invsTarget);
            typeTwo[i].update();
        }

        for (int i = 0; i < mutants.size(); i++) {//mutant player
            int closestFoodDistance = NOFOOD;
            int closestFoodIndex = -1;
            for (int j = 0; j < numFood; j++) {//checks if food is in range
                if (food.ballsX[j] == ATE)
                    continue;
                int foodDistance = (int)mutants[i].getDistanceTo(Vector2({ (float)food.ballsX[j], (float)food.ballsY[j] }));
                if (mutants[i].invsTarget.x == ATE) {
                    mutants[i].invsTarget = Vector2({ (float)(getRandWidth()), (float)(getRandHeight()) });
                }
                if (totalTime - mutants[i].lastEaten < mutants[i].rechargeTime && foodDistance <= 5) {//5 is when player is pretty much on top of the food
                    food.ballsX[j] = ATE;
                    food.ballsY[j] = ATE;
                    mutants[i].foodCnt++;
                    mutants[i].lastEaten = totalTime;
                }else if (totalTime - mutants[i].lastEaten >= mutants[i].rechargeTime && foodDistance <= mutants[i].eatingRange) {
                    food.ballsX[j] = ATE;
                    food.ballsY[j] = ATE;
                    mutants[i].foodCnt++;
                    mutants[i].lastEaten = totalTime;
                    //cout << "TYPE 2 ATE\n";
                }
                if (foodDistance <= mutants[i].detectionRange && foodDistance < closestFoodDistance) {
                    closestFoodDistance = foodDistance;
                    closestFoodIndex = j;
                }
            }
            if (mutants[i].getDistanceTo(mutants[i].invsTarget) <= 5) {//wandering targets
                mutants[i].invsTarget = Vector2({ (float)(getRandWidth()), (float)(getRandHeight()) });
                // cout << "TYPE 2 CAUGHT IT\n";
            }
            if (closestFoodDistance != NOFOOD) {
                mutants[i].invsTarget = Vector2({ (float)(food.ballsX[closestFoodIndex]), (float)(food.ballsY[closestFoodIndex]) });
            }
            mutants[i].seek(mutants[i].invsTarget);
            mutants[i].update();
        }
        //Drawing stuff now ->
        //cout << "DRAW TYPE 1\n";
        for (int i = 0; i < typeOne.size(); i++) {//draws players and their circles
            if (totalTime - typeOne[i].lastEaten >= typeOne[i].rechargeTime)
                DrawCircle(typeOne[i].location.x, typeOne[i].location.y, typeOne[i].eatingRange - 5, RED);
            DrawCircleLines(typeOne[i].location.x, typeOne[i].location.y, typeOne[i].eatingRange - 5, RED);
            typeOne[i].draw(textureType1);
            DrawText(to_string(typeOne[i].foodCnt).c_str(), typeOne[i].location.x - 5, typeOne[i].location.y - 10, 30, BLACK);
        }

        // cout << "DRAW TYPE 2\n";
        for (int i = 0; i < typeTwo.size(); i++) {//draws players and their circles
            DrawCircleLines(typeTwo[i].location.x, typeTwo[i].location.y, typeTwo[i].detectionRange, BLACK);
            typeTwo[i].draw(textureType2);
            DrawText(to_string(typeTwo[i].foodCnt).c_str(), typeTwo[i].location.x - 5, typeTwo[i].location.y - 10, 30, WHITE);
        }

        for (int i = 0; i < mutants.size(); i++) {//draws players and their circles
            if (totalTime - mutants[i].lastEaten >= mutants[i].rechargeTime)
                DrawCircle(mutants[i].location.x, mutants[i].location.y, mutants[i].eatingRange - 5, GREEN);
            DrawCircleLines(mutants[i].location.x, mutants[i].location.y, mutants[i].eatingRange - 5, GREEN);
            DrawCircleLines(mutants[i].location.x, mutants[i].location.y, mutants[i].detectionRange, BLACK);
            mutants[i].draw(textureMutant);
            DrawText(to_string(mutants[i].foodCnt).c_str(), mutants[i].location.x - 5, mutants[i].location.y - 10, 30, BLACK);
        }
        for (int i = 0; i < numFood; i++) {//draws food circles
            if (food.ballsX[i] != ATE) {
                DrawCircle(food.ballsX[i], food.ballsY[i], 5, WHITE);
            }
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}