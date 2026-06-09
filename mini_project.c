#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 30
#define COLS 60
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef enum {
    LINE = 1,
    RECTANGLE,
    TRIANGLE,
    CIRCLE
} ObjectType;

typedef struct {
    int type;

    int x1, y1;
    int x2, y2;
    int x3, y3;

    int radius;
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

void initializeCanvas() {
    int i, j;

    for(i=0;i<ROWS;i++)
        for(j=0;j<COLS;j++)
            canvas[i][j] = '_';
}

void displayCanvas() {
    int i,j;

    printf("\n");

    for(i=0;i<ROWS;i++) {
        for(j=0;j<COLS;j++)
            printf("%c", canvas[i][j]);

        printf("\n");
    }
}

void putPixel(int x,int y,char ch) {
    if(x>=0 && x<COLS && y>=0 && y<ROWS)
        canvas[y][x] = ch;
}

void drawLineRaw(int x1,int y1,int x2,int y2,char ch) {

    int dx = abs(x2-x1);
    int dy = abs(y2-y1);

    int sx = (x1 < x2)?1:-1;
    int sy = (y1 < y2)?1:-1;

    int err = dx - dy;

    while(1) {

        putPixel(x1,y1,ch);

        if(x1==x2 && y1==y2)
            break;

        int e2 = 2*err;

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

void drawRectangleRaw(int x1,int y1,int x2,int y2,char ch) {

    drawLineRaw(x1,y1,x2,y1,ch);
    drawLineRaw(x2,y1,x2,y2,ch);
    drawLineRaw(x2,y2,x1,y2,ch);
    drawLineRaw(x1,y2,x1,y1,ch);
}

void drawTriangleRaw(
        int x1,int y1,
        int x2,int y2,
        int x3,int y3,
        char ch) {

    drawLineRaw(x1,y1,x2,y2,ch);
    drawLineRaw(x2,y2,x3,y3,ch);
    drawLineRaw(x3,y3,x1,y1,ch);
}

void plotCirclePoints(int xc,int yc,int x,int y,char ch) {

    putPixel(xc+x,yc+y,ch);
    putPixel(xc-x,yc+y,ch);
    putPixel(xc+x,yc-y,ch);
    putPixel(xc-x,yc-y,ch);

    putPixel(xc+y,yc+x,ch);
    putPixel(xc-y,yc+x,ch);
    putPixel(xc+y,yc-x,ch);
    putPixel(xc-y,yc-x,ch);
}

void drawCircleRaw(int xc,int yc,int r,char ch) {

    int x=0;
    int y=r;

    int d=1-r;

    while(x<=y) {

        plotCirclePoints(xc,yc,x,y,ch);

        if(d<0)
            d += 2*x + 3;
        else {
            d += 2*(x-y) + 5;
            y--;
        }

        x++;
    }
}

void drawObject(Object obj,char ch) {

    switch(obj.type) {

        case LINE:
            drawLineRaw(
                obj.x1,obj.y1,
                obj.x2,obj.y2,
                ch);
            break;

        case RECTANGLE:
            drawRectangleRaw(
                obj.x1,obj.y1,
                obj.x2,obj.y2,
                ch);
            break;

        case TRIANGLE:
            drawTriangleRaw(
                obj.x1,obj.y1,
                obj.x2,obj.y2,
                obj.x3,obj.y3,
                ch);
            break;

        case CIRCLE:
            drawCircleRaw(
                obj.x1,obj.y1,
                obj.radius,
                ch);
            break;
    }
}

void addObject() {

    Object obj;

    int centerX = COLS / 2;
    int centerY = ROWS / 2;

    printf("\n1.Line");
    printf("\n2.Rectangle");
    printf("\n3.Triangle");
    printf("\n4.Circle");
    printf("\nChoice: ");
    scanf("%d",&obj.type);

    switch(obj.type) {

        case LINE:
        {
            int length;

            printf("Length: ");
            scanf("%d",&length);

            obj.x1 = centerX - length/2;
            obj.y1 = centerY;

            obj.x2 = centerX + length/2;
            obj.y2 = centerY;
            break;
        }

        case RECTANGLE:
        {
            int width,height;

            printf("Width Height: ");
            scanf("%d%d",&width,&height);

            obj.x1 = centerX - width/2;
            obj.y1 = centerY - height/2;

            obj.x2 = centerX + width/2;
            obj.y2 = centerY + height/2;
            break;
        }

        case TRIANGLE:
        {
            int size;

            printf("Size: ");
            scanf("%d",&size);

            obj.x1 = centerX;
            obj.y1 = centerY - size;

            obj.x2 = centerX - size;
            obj.y2 = centerY + size;

            obj.x3 = centerX + size;
            obj.y3 = centerY + size;
            break;
        }

        case CIRCLE:
            printf("Radius: ");
            scanf("%d",&obj.radius);

            obj.x1 = centerX;
            obj.y1 = centerY;
            break;

        default:
            return;
    }

    objects[objectCount] = obj;

    drawObject(obj,'*');

    objectCount++;
}

void deleteObject() {

    int id;

    printf("Object ID (0-%d): ", objectCount - 1);
    scanf("%d", &id);

    if(id < 0 || id >= objectCount)
        return;

    for(int i = id; i < objectCount - 1; i++)
        objects[i] = objects[i + 1];

    objectCount--;

    initializeCanvas();

    for(int i = 0; i < objectCount; i++)
        drawObject(objects[i], '*');
}

void modifyObject() {

    int id;

    printf("Object ID (0-%d): ", objectCount - 1);
    scanf("%d", &id);

    if(id < 0 || id >= objectCount)
        return;

    Object obj = objects[id];

    switch(obj.type) {

        case LINE:
        {
            int length;

            printf("New Length: ");
            scanf("%d", &length);

            obj.x1 = COLS/2 - length/2;
            obj.y1 = ROWS/2;

            obj.x2 = COLS/2 + length/2;
            obj.y2 = ROWS/2;
            break;
        }

        case RECTANGLE:
        {
            int width, height;

            printf("New Width Height: ");
            scanf("%d%d", &width, &height);

            obj.x1 = COLS/2 - width/2;
            obj.y1 = ROWS/2 - height/2;

            obj.x2 = COLS/2 + width/2;
            obj.y2 = ROWS/2 + height/2;
            break;
        }

        case TRIANGLE:
        {
            int size;

            printf("New Size: ");
            scanf("%d", &size);

            obj.x1 = COLS/2;
            obj.y1 = ROWS/2 - size;

            obj.x2 = COLS/2 - size;
            obj.y2 = ROWS/2 + size;

            obj.x3 = COLS/2 + size;
            obj.y3 = ROWS/2 + size;
            break;
        }

        case CIRCLE:
        {
            printf("New Radius: ");
            scanf("%d", &obj.radius);

            obj.x1 = COLS/2;
            obj.y1 = ROWS/2;
            break;
        }
    }

    objects[id] = obj;

    initializeCanvas();

    for(int i = 0; i < objectCount; i++)
        drawObject(objects[i], '*');
}

int main() {

    int choice;

    initializeCanvas();

    do {

        printf("\n\n==== 2D GRAPHICS EDITOR ====\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Canvas\n");
        printf("5. Clear Canvas\n");
        printf("6. Exit\n");

        printf("Choice: ");
        scanf("%d",&choice);

        switch(choice) {

            case 1:
                addObject();
                break;

            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;

            case 4:
                displayCanvas();
                break;

            case 5:
                initializeCanvas();
                break;
        }

    } while(choice != 6);

    return 0;
}