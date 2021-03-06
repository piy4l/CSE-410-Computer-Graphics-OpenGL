#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle, cAngle;
int drawgrid;
int drawaxes;
double angle,change;
double sphereRadius, cubeEdge;


struct point
{
	double x,y,z;
} pos, u, r, l, temp;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f(a/2.0, a/2.0, 0);
		glVertex3f(a/2.0, -a/2.0, 0);
		glVertex3f(-a/2.0, -a/2.0, 0);
		glVertex3f(-a/2.0, a/2.0, 0);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCube(double edge){
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);{
        glVertex3f(edge, 0, 0);
        glVertex3f(edge, edge, 0);
        glVertex3f(0, edge, 0);
        glVertex3f(0, 0, 0);
    }glEnd();
    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);{
        glVertex3f(edge, 0, 0);
        glVertex3f(edge, 0, edge);
        glVertex3f(0, 0, edge);
        glVertex3f(0, 0, 0);
    }glEnd();
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);{
        glVertex3f(0, 0, edge);
        glVertex3f(0, edge, edge);
        glVertex3f(0, edge, 0);
        glVertex3f(0, 0, 0);
    }glEnd();
    glColor3f(1, 1, 0);
    glBegin(GL_QUADS);{
        glVertex3f(edge, 0, 0);
        glVertex3f(edge, edge, 0);
        glVertex3f(edge, edge, edge);
        glVertex3f(edge, 0, edge);
    }glEnd();
    glColor3f(1, 0, 1);
    glBegin(GL_QUADS);{
        glVertex3f(0, edge, 0);
        glVertex3f(edge, edge, 0);
        glVertex3f(edge, edge, edge);
        glVertex3f(0, edge, edge);
    }glEnd();
    glColor3f(0, 1, 1);
    glBegin(GL_QUADS);{
        glVertex3f(0, 0, edge);
        glVertex3f(edge, 0, edge);
        glVertex3f(edge, edge, edge);
        glVertex3f(0, edge, edge);
    }glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);{
        glVertex3f(edge, edge, 0);
        glVertex3f(edge, 0, 0);
        glVertex3f(2*edge, 0, 0);
        glVertex3f(2*edge, edge, 0);
    }glEnd();
}


void drawOneEighthOfSphere(double radius, int slices, int stacks){
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
			points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawOneFourthOfCylinder(double radius, double height, int segments){
    int i;
    //double shade = 120;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*(pi/2));
        points[i].y=radius*sin(((double)i/(double)segments)*(pi/2));
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        //if(i<segments/2)shade=2*(double)i/(double)segments;
        //else shade=2*(1.0-(double)i/(double)segments);
        //glColor3f(1,0,0);

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,height/2.0);
			glVertex3f(points[i+1].x,points[i+1].y,height/2.0);

            glVertex3f(points[i+1].x,points[i+1].y,-height/2.0);
			glVertex3f(points[i].x,points[i].y,-height/2.0);

        }
        glEnd();
    }
}

void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    //glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);

}

void drawSphereCube(double radius, double edge, int segments){

    double dist = edge/2.0 - radius;
    //part - 1 : drawing one eighth of spheres
    glColor3f(255, 0,0);
    glPushMatrix();
    glTranslatef(dist, dist, dist);
    drawOneEighthOfSphere(radius, segments, segments);
    glPopMatrix();

    //drawCube(10);

    glPushMatrix();
    glTranslatef(-dist, dist, dist);
    glRotatef(90, 0, 0, 1);
    drawOneEighthOfSphere(radius, segments, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-dist, -dist, dist);
    glRotatef(180, 0, 0, 1);
    drawOneEighthOfSphere(radius, segments, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(dist, -dist, dist);
    glRotatef(270, 0, 0, 1);
    drawOneEighthOfSphere(radius, segments, segments);
    glPopMatrix();

    //get into upside-down
    glPushMatrix();
    glRotatef(180, 1, 1, 0);


    glPushMatrix();
    glTranslatef(dist, dist, dist);
    drawOneEighthOfSphere(radius, segments, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-dist, dist, dist);
    glRotatef(90, 0, 0, 1);
    drawOneEighthOfSphere(radius, segments, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-dist, -dist, dist);
    glRotatef(180, 0, 0, 1);
    drawOneEighthOfSphere(radius, segments, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(dist, -dist, dist);
    glRotatef(270, 0, 0, 1);
    drawOneEighthOfSphere(radius, segments, segments);
    glPopMatrix();

    //getting out from upside down
    glPopMatrix();


    //part - 2 : drawing squares
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef(0, 0, edge/2);
    drawSquare(edge - 2*radius);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -edge/2);
    drawSquare(edge - 2*radius);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, edge/2, 0);
    glRotatef(90, 1, 0, 0);
    drawSquare(edge - 2*radius);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -edge/2, 0);
    glRotatef(90, 1, 0, 0);
    drawSquare(edge - 2*radius);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(edge/2, 0, 0);
    glRotatef(90, 0, 1, 0);
    drawSquare(edge - 2*radius);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-edge/2, 0, 0);
    glRotatef(90, 0, 1, 0);
    drawSquare(edge - 2*radius);
    glPopMatrix();


    //part - 3 : drawing one fourth of cylinders
    glColor3f(0, 128,0);
    glPushMatrix();
    glTranslatef(dist, dist, 0);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-dist, dist, 0);
    glRotatef(90, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-dist, -dist, 0);
    glRotatef(180, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(dist, -dist, 0);
    glRotatef(270, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(dist, 0, dist);
    glRotatef(45, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-dist, 0, dist);
    glRotatef(45, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45 + 90, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-dist, 0, -dist);
    glRotatef(45, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45 + 180, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(dist, 0, -dist);
    glRotatef(45, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45 + 270, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0, dist, dist);
    glRotatef(90, 0, 0, 1);
    glRotatef(45, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -dist, dist);
    glRotatef(-90, 0, 0, 1);
    glRotatef(45, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, dist, -dist);
    glRotatef(-90, 0, 0, 1);
    glRotatef(45, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -dist, -dist);
    glRotatef(90, 0, 0, 1);
    glRotatef(45, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    drawOneFourthOfCylinder(radius, edge - 2*radius, segments);
    glPopMatrix();

}

point getCrossProduct(point p, point q){
    point crossProduct;
    crossProduct.x = p.y * q.z - p.z * q.y;
    crossProduct.y = p.z * q.x - p.x * q.z;
    crossProduct.z = p.x * q.y - p.y * q.x;
    return crossProduct;
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

         case '1': //left rotate
			//drawgrid=1-drawgrid;
			l.x = l.x * cos(-1.0*cameraAngle) + r.x * sin(-1.0*cameraAngle);
            l.y = l.y * cos(-1.0*cameraAngle) + r.y * sin(-1.0*cameraAngle);
            l.z = l.z * cos(-1.0*cameraAngle) + r.z * sin(-1.0*cameraAngle);
            r = getCrossProduct(l, u);
			break;

		case '2': //right rotate
			l.x = l.x * cos(cameraAngle) + r.x * sin(cameraAngle);
            l.y = l.y * cos(cameraAngle) + r.y * sin(cameraAngle);
            l.z = l.z * cos(cameraAngle) + r.z * sin(cameraAngle);
            r = getCrossProduct(l, u);
			break;

        case '3': //look up
			l.x = l.x * cos(cameraAngle) + u.x * sin(cameraAngle);
            l.y = l.y * cos(cameraAngle) + u.y * sin(cameraAngle);
            l.z = l.z * cos(cameraAngle) + u.z * sin(cameraAngle);
            u = getCrossProduct(r, l);
			break;

        case '4': //look down
            l.x = l.x * cos(-1.0*cameraAngle) + u.x * sin(-1.0*cameraAngle);
            l.y = l.y * cos(-1.0*cameraAngle) + u.y * sin(-1.0*cameraAngle);
            l.z = l.z * cos(-1.0*cameraAngle) + u.z * sin(-1.0*cameraAngle);
            u = getCrossProduct(r, l);
			break;

        case '5': //tilt clockwise
			r.x = r.x * cos(-1.0*cameraAngle) + u.x * sin(-1.0*cameraAngle);
            r.y = r.y * cos(-1.0*cameraAngle) + u.y * sin(-1.0*cameraAngle);
            r.z = r.z * cos(-1.0*cameraAngle) + u.z * sin(-1.0*cameraAngle);
            u = getCrossProduct(r, l);
			break;

        case '6': //tilt counterclockwise
			r.x = r.x * cos(cameraAngle) + u.x * sin(cameraAngle);
            r.y = r.y * cos(cameraAngle) + u.y * sin(cameraAngle);
            r.z = r.z * cos(cameraAngle) + u.z * sin(cameraAngle);
            u = getCrossProduct(r, l);
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x, int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			pos.x -= l.x * change;
            pos.y -= l.y * change;
            pos.z -= l.z * change;
			break;

		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			pos.x += l.x * change;
            pos.y += l.y * change;
            pos.z += l.z * change;
			break;

		case GLUT_KEY_RIGHT:
			cAngle += 0.03;
			pos.x += r.x * change;
            pos.y += r.y * change;
            pos.z += r.z * change;
			break;

		case GLUT_KEY_LEFT:
			cAngle -= 0.03;
			pos.x -= r.x * change;
            pos.y -= r.y * change;
            pos.z -= r.z * change;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x += u.x * change;
		    pos.y += u.y * change;
		    pos.z += u.z * change;
			break;

		case GLUT_KEY_PAGE_DOWN:
			pos.x -= u.x * change;
			pos.y -= u.y * change;
			pos.z -= u.z * change;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    /*if(cubeEdge > 0){
                sphereRadius++;
                cubeEdge--;
		    }*/
		    if(sphereRadius < cubeEdge/2){
                sphereRadius++;
		    }
			break;
		case GLUT_KEY_END:
		    if(sphereRadius > 0){
                sphereRadius--;
                //cubeEdge++;
		    }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cAngle), 200*sin(cAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z,	pos.x + l.x, pos.y + l.y, pos.z + l.z,	u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,30);

    //drawCube(50);

    //drawOneEighthOfSphere(30, 30, 30);

    //drawOneFourthOfCylinder(20, 30, 30);

    drawSphereCube(sphereRadius, cubeEdge, 30);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=1;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
    pos = {100.0, 100.0, 50.0};
    u = {0.0, 0.0, 1.0};
    r = {-1/sqrt(2), 1/sqrt(2), 0.0};
    l = {-1/sqrt(2), -1/sqrt(2), 0.0};

    /*
    pos = {100, 0, 0};
    u = {0, 0, 1};
    r = {0, 1, 0};
    l = {-1, 0, 0};
    */

	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=0.01;
	angle=0;
	change = 5.0;

	sphereRadius = 10.0;
	cubeEdge = 50.0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
