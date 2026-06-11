#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 24
#define MAX_OBJECTS 100

#define EMPTY '_'
#define PIXEL '*'

typedef enum {
    LINE = 1,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct {
    ShapeType type;

    int x1, y1, x2, y2;
    int x3, y3;

    int radius;

    int active;
} Object;

char picture[HEIGHT][WIDTH];
Object objects[MAX_OBJECTS];
int objectCount = 0;

void clearPicture() {
    int i, j;

    for(i = 0; i < HEIGHT; i++) {
        for(j = 0; j < WIDTH; j++) {
            picture[i][j] = EMPTY;
        }
    }
}

void setPixel(int x, int y) {
    if(x >= 0 && x < WIDTH &&
       y >= 0 && y < HEIGHT) {
        picture[y][x] = PIXEL;
    }
}

void displayPicture() {
    int i, j;

    for(i = 0; i < HEIGHT; i++) {
        for(j = 0; j < WIDTH; j++) {
            printf("%c", picture[i][j]);
        }
        printf("\n");
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1) {
        setPixel(x1, y1);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    drawLine(x1, y1, x2, y1);
    drawLine(x2, y1, x2, y2);
    drawLine(x2, y2, x1, y2);
    drawLine(x1, y2, x1, y1);
}

void drawCircle(int cx, int cy, int radius) {
    int x, y;

    for(y = cy - radius; y <= cy + radius; y++) {
        for(x = cx - radius; x <= cx + radius; x++) {

            double dist =
                sqrt((x - cx) * (x - cx) +
                     (y - cy) * (y - cy));

            if(fabs(dist - radius) <= 0.5) {
                setPixel(x, y);
            }
        }
    }
}

void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3) {

    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void renderObjects() {
    int i;

    clearPicture();

    for(i = 0; i < objectCount; i++) {

        if(!objects[i].active)
            continue;

        switch(objects[i].type) {

            case LINE:
                drawLine(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case RECTANGLE:
                drawRectangle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case CIRCLE:
                drawCircle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].radius
                );
                break;

            case TRIANGLE:
                drawTriangle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2,
                    objects[i].x3,
                    objects[i].y3
                );
                break;
        }
    }
}

void addObject() {

    Object obj;
    int shape;

    printf("\nChoose shape type:\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");

    printf("Enter shape type: ");
    scanf("%d", &shape);

    obj.type = shape;
    obj.active = 1;

    if(shape == LINE) {

        printf("Enter x1 y1 x2 y2: ");
        scanf("%d %d %d %d",
              &obj.x1, &obj.y1,
              &obj.x2, &obj.y2);
    }
    else if(shape == RECTANGLE) {

        printf("Enter top-left x y and bottom-right x y: ");
        scanf("%d %d %d %d",
              &obj.x1, &obj.y1,
              &obj.x2, &obj.y2);
    }
    else if(shape == CIRCLE) {

        printf("Enter center x y and radius: ");
        scanf("%d %d %d",
              &obj.x1, &obj.y1,
              &obj.radius);
    }
    else if(shape == TRIANGLE) {

        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d %d %d %d %d %d",
              &obj.x1, &obj.y1,
              &obj.x2, &obj.y2,
              &obj.x3, &obj.y3);
    }

    objects[objectCount] = obj;

    printf("Object added with index %d.\n",
           objectCount);

    objectCount++;
}

void deleteObject() {
    int index;

    printf("Enter object index: ");
    scanf("%d", &index);

    if(index >= 0 &&
       index < objectCount &&
       objects[index].active) {

        objects[index].active = 0;
        printf("Object deleted.\n");
    }
}

void modifyObject() {
    int index;

    printf("Enter object index: ");
    scanf("%d", &index);

    if(index < 0 ||
       index >= objectCount ||
       !objects[index].active)
        return;

    Object *obj = &objects[index];

    if(obj->type == LINE) {

        printf("Enter x1 y1 x2 y2: ");
        scanf("%d %d %d %d",
              &obj->x1, &obj->y1,
              &obj->x2, &obj->y2);
    }
    else if(obj->type == RECTANGLE) {

        printf("Enter top-left x y and bottom-right x y: ");
        scanf("%d %d %d %d",
              &obj->x1, &obj->y1,
              &obj->x2, &obj->y2);
    }
    else if(obj->type == CIRCLE) {

        printf("Enter center x y and radius: ");
        scanf("%d %d %d",
              &obj->x1, &obj->y1,
              &obj->radius);
    }
    else if(obj->type == TRIANGLE) {

        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d %d %d %d %d %d",
              &obj->x1, &obj->y1,
              &obj->x2, &obj->y2,
              &obj->x3, &obj->y3);
    }

    printf("Object modified.\n");
}

void listObjects() {
    int i;

    for(i = 0; i < objectCount; i++) {

        if(objects[i].active)
            printf("Index %d Type %d\n",
                   i, objects[i].type);
    }
}

int main() {

    int choice;

    while(1) {

        printf("2D Graphics Editor\n");
        printf("Canvas size: %d x %d\n",
               WIDTH, HEIGHT);

        printf("1. Add object\n");
        printf("2. Delete object\n");
        printf("3. Modify object\n");
        printf("4. Display picture\n");
        printf("5. List objects\n");
        printf("0. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        if(choice == 1) {
            addObject();
        }
        else if(choice == 2) {
            deleteObject();
        }
        else if(choice == 3) {
            modifyObject();
        }
        else if(choice == 4) {
            renderObjects();
            printf("\n");
            displayPicture();
        }
        else if(choice == 5) {
            listObjects();
        }
        else if(choice == 0) {
            printf("Goodbye.\n");
            break;
        }

        printf("\n");
    }

    return 0;
}