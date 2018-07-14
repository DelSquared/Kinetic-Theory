#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//be sure to include GLFW. I used this video https://www.youtube.com/watch?v=0WrSGMuU964

void particle(float x, float y,float rad,float r, float g, float b); //functions for drawing particles
void Menu(); //menu function
void SetupCondens(); //setup functions called by Menu(). More coming soon.

class Particle
{
    public:
    //phase space and colour space
    float x,y,vx,vy,r,g,b;

    //object functions
    float Collision(Particle p){
       return (x-p.x)*(x-p.x)+(y-p.y)*(y-p.y);
    }
    
    void Step(){
       x+=0.005*vx;
       y+=0.005*vy;
    }
};

float r = 0.01; //radius
float damping = 0.01; //damping

int main(void)
{
    srand(time(NULL)); //initialisations
    int n=500;
    Particle p[n];
    int windowSize = 600; //setting up canvas
    for (int i=0;i<n;i++){
            p[i].x=2*((float)rand()/(float)RAND_MAX)-1;
            p[i].y=2*((float)rand()/(float)RAND_MAX)-1;
            p[i].vx=((float)rand()/(float)RAND_MAX)-0.5;
            p[i].vy=((float)rand()/(float)RAND_MAX)-0.5;
            p[i].r=(float)rand()/(float)RAND_MAX;
            p[i].g=(float)rand()/(float)RAND_MAX;
            p[i].b=(float)rand()/(float)RAND_MAX;
    };
    Menu(); //Initialising menu

    GLFWwindow* window;

    //Initialize the library
    if (!glfwInit())
        return -1;

    //Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(windowSize, windowSize, "Kinetic", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current
    glfwMakeContextCurrent(window);

    //Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i=0;i<n;i++){ //Wall collision. The wals are the only potential inelastic collisions to resemble cooling
            p[i].Step();
            if (p[i].x<=-1 || p[i].x>=+1){
                p[i].vx=-damping*p[i].vx;
                p[i].Step();
                p[i].Step();
            }
            if (p[i].y<=-1 || p[i].y>=+1){
                p[i].vy=-damping*p[i].vy;
                p[i].Step();
                p[i].Step();
            }
            particle(p[i].x,p[i].y,r,p[i].r,p[i].g,p[i].b); //drawing

        };
        for (int i=0;i<n;i++){ //binary particle collisions
            for (int j=i;j<n;j++){
                if (p[i].Collision(p[j])<=0.05*r){
                    float sx=p[i].vx+p[j].vx,sy=p[i].vy+p[j].vy;
                    p[i].vx=sx-p[i].vx;
                    p[i].vy=sy-p[i].vy;
                    p[j].vx=sx-p[j].vx;
                    p[j].vy=sy-p[j].vy;
                }
            };
        };

        //Swap front and back buffers
        glfwSwapBuffers(window);

        //Poll for and process events
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

//Particle rendering function
void particle(float x, float y,float rad,float r, float g, float b) { 
    glColor3f (r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(-rad+x,rad+y);
    glVertex2f(rad+x,rad+y);
    glVertex2f(rad+x,-rad+y);
    glVertex2f(-rad+x,-rad+y);
    glEnd();
}

//Menu function
void Menu(){
    printf("CLASSICAL GAS SIMULATION (select option):\n\n");
    printf("1. Condensation/Evaporation\n");
    printf("\n(More coming soon)\n");
    printf("\nYour selection: ");
    int selection;
    scanf("%d",&selection);
    switch(selection) {
        case 1:
            SetupCondens();
            break; //optional
        default:
            printf("Unavailable option\n");
            Menu();
    }
}

//Condensation setup function called from Menu()
void SetupCondens(){
    printf("\nInput wall-damping coefficient: ");
    scanf("%f",&damping);
    damping=abs(damping);
    printf("\nStarting...\n");
}
