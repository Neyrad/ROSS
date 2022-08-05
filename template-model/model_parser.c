#include "model.h"

void PrintMap()
{
    printf("storage [%d, %d]\n", storage.height, storage.length);
    for (int y = 0; y < storage.height; ++y) {
        for (int x = 0; x < storage.length; ++x)
            printf("%d ", storage.data[y][x]);
        printf("\n");
    }
}

//Converts a CSV file to a 2D int array
void parse(char* path)
{
    struct stat stats;
    stat(path, &stats);
    FILE* f = fopen(path, "r");
    assert(f); //to check if the path is correct
    char buf[MAX_ROOM_HEIGHT * MAX_ROOM_LENGTH * 2 + MAX_COMMENT_LENGTH]; // * 2 because of commas
    fread(buf, sizeof(buf[0]), stats.st_size, f);
    fclose(f);

    int start_index = 0;
    if (buf[0] == '#') {
        for (start_index = 0; buf[start_index] != '\n' ; ++start_index)
            ;
        ++start_index; //next symbol after #bla-bla-bla\n
    }
    
    storage.height = 0;
    int x = 0;
    for (int i = start_index; i < stats.st_size; ++i) {
        if (buf[i] == '\n') {
            ++storage.height;
            storage.length = x;
            x = 0;
            continue;
        }
        if (isdigit(buf[i])) {
            storage.data[storage.height][x] = buf[i] - '0';
            ++x;
        }
    }
}

void RobotsInit()
{
    Robots.N = 0;
    for (int y = 0; y < storage.height; ++y)
        for (int x = 0; x < storage.length; ++x)
            if (storage.data[y][x] == CELL_ROBOT_UP    || \
                storage.data[y][x] == CELL_ROBOT_DOWN  || \
                storage.data[y][x] == CELL_ROBOT_LEFT  || \
                storage.data[y][x] == CELL_ROBOT_RIGHT    ) {
                
                Robots.data[Robots.N].x = x;
                Robots.data[Robots.N].y = y;
                
                switch(storage.data[y][x])
                {
                    case CELL_ROBOT_UP:
                        Robots.data[Robots.N].is_facing = UP;
                        break;
                    case CELL_ROBOT_DOWN:
                        Robots.data[Robots.N].is_facing = DOWN;
                        break;
                    case CELL_ROBOT_LEFT:
                        Robots.data[Robots.N].is_facing = LEFT;
                        break;
                    case CELL_ROBOT_RIGHT:
                        Robots.data[Robots.N].is_facing = RIGHT;
                        break;
                }
                ++Robots.N;
            }
}

void RobotsPrint()
{
    for (int i = 0; i < Robots.N; ++i) {
        printf("Robot #%d is located at (%d, %d) and is facing ", \
                      i+1, Robots.data[i].x, Robots.data[i].y);
        
        switch(Robots.data[i].is_facing)
        {
            case UP:
                printf("UP\n");
                break;
            case DOWN:
                printf("DOWN\n");
                break;
            case LEFT:
                printf("LEFT\n");
                break;
            case RIGHT:
                printf("RIGHT\n");
                break;
        } 
    }
}
