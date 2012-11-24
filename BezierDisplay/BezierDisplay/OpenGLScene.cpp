/*
 COMS4160: Computer Graphics
 Theme 5, Milestone 1.
 Bezier Curve.
 
 Implemented by Papoj Thamjaroenporn.
 10/30/11
 */


#include "OpenGLScene.h"
#include "OpenGLDisplayController.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Rendering state
//bool g_rendering_enabled = true;
//double g_sec_per_frame;

OpenGLDisplayController g_display_controller( 512, 512 );
renderingutils::Color g_bgColor( 1.0, 1.0, 1.0 );

SceneData g_scene_data;



////Light enabling variables.
//bool light_0_on = true;  //By default, light 0 will be on.
//bool light_1_on = false;
//
////Shading mode variable. In this assignment, we only use GL_FLAT and GL_SMOOTH.
//bool isFlat = false;  //By default, the shading model is flat mode.
//
////Windows attribute
//int width = 920;
//int height = 690;
//int w, h;	//Dynamic sizes of width and height. Used for reshaping.
//int border = 5;
//int mainWindow;  //"Signature" number for the main window.
//int * viewWindows;
//float near_plane = 1.0;
//float far_plane = 1000.0;
///*
////Represents the camera in each subwindow.
////0 -> Top
////1 -> Perspective
////2 -> Front
////3 -> Side
//int cameraNumber = 0;
//int channels = 4;
//
////Orthographic camera attributes.
//float originalOrthoW = 80.0;
//float originalOrthoH = 60.0;
//int left_s = -originalOrthoW / 2;
//int right_s = originalOrthoW / 2;
//int top_s = originalOrthoH / 2;
//int bottom_s = -originalOrthoH / 2;
// */
////Perspective
//int viewAngle = 60;
//float zoomFactor = 0.1;
//float angle = 0.0;
//
////Main menu signatures for each subwindow.
//int mainMenu;
//int lightMenu;
//int colorMenu;
//
////Floor attributes.
//int floorX1 = -30;
//int floorX2 = 30;
//int floorZ1 = -250;
//int floorZ2 = 250;
//int floorGridW = 5;
//
////Sphere atts.
//int Nslice = 30;
//int Nstack = 40;
//
////The light source attributes.
//GLfloat ambientLight0[] = {0.35, 0.25, 0.25, 1.0};
//GLfloat diffuseLight0[] = {1.0, 1.0, 1.0, 1.0};
//GLfloat specularLight0[] = {1.0, 1.0, 1.0, 1.0};
//GLfloat pos0[] = {100, 10000, 0};
//
//GLfloat ambientLight1[] = {0.1, 0.00, 0.00, 1.0};
//GLfloat diffuseLight1[] = {1.0, 1.0, 1.0, 1.0};
//GLfloat specularLight1[] = {1.0, 0.0, 1.0, 1.0};
//GLfloat pos1[] = {-100, 10000, 0};
//
//GLfloat ambientLight2[] = {0.6, 0.3, 0.1, 1.0};
//GLfloat diffuseLight2[] = {0.0, 0.5, 0.0, 1.0};
//GLfloat specularLight2[] = {1.0, 1.0, 1.0, 1.0};
//GLfloat pos2[] = {600, 10, 500};
//
//GLfloat ambientLight3[] = {0.7, 0.6, 1.0, 1.0};
//GLfloat diffuseLight3[] = {0.1, 1.0, 0.3, 1.0};
//GLfloat specularLight3[] = {1.0, 1.0, 0.0, 1.0};
//GLfloat pos3[] = {-600, 10, -500};
//
////The material attributes. The initial attributes are Lambertian reflectance.
//GLfloat blankMaterial[] = {0.0, 0.0, 0.0, 1.0};
////-->Lambertian
//GLfloat specularLambertMaterial[] = {0.0, 0.0, 0.0, 1.0};  //Zero specular.
////-->Blinn-Phong
//int shineValues[] = {2, 8, 16, 64, 128};  
//int expoSelection = 4; //By default, choose index 0, which yields 2.
//GLfloat specularPhongMaterial[] = {1.0, 1.0, 1.0, 1.0};  //White Specular.
//GLfloat shininess[] = {static_cast<GLfloat>(shineValues[expoSelection])};  //Phong exponents.
////-->Blinn-Phong with Emissive.
//GLfloat emissiveMaterial[] = {0.9, 0.6, 0.6, 1.0};  //White Emission.
//
//GLfloat specularBodyMaterial[] = {0.8, 0.8, 0.8, 1.0};  //Phong Specular.
//
//GLfloat specularGlassMaterial[] = {1.0, 1.0, 1.0};
//GLfloat diffuseGlassMaterial[] = {0.0, 0.0, 0.0};
//GLfloat ambientGlassMaterial[] = {0.0, 0.0, 0.0};
//
//GLuint theScene;			//For Display Lists.

//int y = 15;
//int z = 0;
//
//typedef list<BezierCurve> * BezierCurveListPtr;
//typedef list<BezierSurface> * BezierSurfaceListPtr;
//BezierCurveListPtr curves_gl;
//BezierSurfaceListPtr surfaces_gl;
//
//int LOD = 1;
//
//void getBezierInput(list<BezierCurve> &cs, list<BezierSurface> &ss) {
//	curves_gl = &cs;
//	surfaces_gl = &ss;
//}



////////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL rendering and UI functions

//Called when the window size is changed.
void reshape (int w, int h)
{
	g_display_controller.reshape( w, h );
	
	assert( renderingutils::checkGLErrors() );
}

void setOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	gluOrtho2D(0, g_display_controller.getWindowWidth(), 0, g_display_controller.getWindowHeight());
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	assert( renderingutils::checkGLErrors() );
}

void drawHUD()
{

}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT );
	
	glMatrixMode( GL_MODELVIEW );
	
	//g_bezier_renderer->renderScene();
	drawGrid(*(g_scene_data.gridSizeX), *(g_scene_data.gridSizeY), 0.5);
	
	//drawHUD();
	
	glutSwapBuffers();
	
	assert( renderingutils::checkGLErrors() );
	
}

// TODO: Implement this
void centerCamera()
{

}

void keyboard( unsigned char key, int x, int y )
{
	g_display_controller.keyboard( key, x, y );
	
	if( key == 'q' )
	{
		exit(0);
	}
	else if(key == 'c' || key == 'C' )
	{
		centerCamera();
		g_display_controller.reshape( g_display_controller.getWindowWidth(),
									  g_display_controller.getWindowHeight() );
		glutPostRedisplay();
	}
	
	assert( renderingutils::checkGLErrors() );
}

void special( int key, int x, int y )
{
	g_display_controller.special( key, x, y );
	
	assert( renderingutils::checkGLErrors() );
}

void mouse( int button, int state, int x, int y )
{
	g_display_controller.mouse( button, state, x, y );
	
	assert( renderingutils::checkGLErrors() );
}

void motion( int x, int y )
{
	g_display_controller.motion( x, y );
	
	assert( renderingutils::checkGLErrors() );
}

void idle()
{
	glutPostRedisplay();
	
	assert( renderingutils::checkGLErrors() );
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// initializeOpenGLandGLUT and runOpenGL methods
void initializeOpenGLandGLUT ( int argc, char** argv ) {

	// Set camera at center of the scene
	centerCamera();

	// Initialize GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );					// Buffer and color
	glutInitWindowSize( g_display_controller.getWindowWidth(),
						g_display_controller.getWindowHeight() );
	glutCreateWindow("Bezier Curve Display");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(idle);
  
	// Initialize OpenGL
	reshape(g_display_controller.getWindowWidth(),g_display_controller.getWindowHeight());
	glClearColor(g_bgColor.r, g_bgColor.g, g_bgColor.b, 1.0);
  
	assert( renderingutils::checkGLErrors() );
		
//    glEnable (GL_LIGHTING);			//Allow lighting for the scene.
//	glEnable (GL_NORMALIZE);		//Recalculate the matrix-transformed normal vectors.
//	glEnable (GL_DEPTH_TEST);		//Allow depth test.
//	glEnable (GL_COLOR_MATERIAL);	//Allow the material to be colored.
//	glEnable (GL_BLEND);			//Enable transparent material.
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	
//	glEnable(GL_LIGHT2);
//	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight3);
//	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight3);
//	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight3);
//	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	
}

void runOpenGL ()
{
	glutMainLoop ();
}

//void light0 (void) {
//	if (light_0_on) {
//		glEnable(GL_LIGHT0);
//		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
//		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
//		glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
//		glLightfv(GL_LIGHT0, GL_POSITION, pos0);
//	} else {
//		glDisable(GL_LIGHT0);
//	}
//	
//}
//
//void light1 (void) {
//	if (light_1_on) {
//		glEnable(GL_LIGHT1);
//		glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
//		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
//		glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
//		glLightfv(GL_LIGHT1, GL_POSITION, pos1);
//	} else {
//		glDisable(GL_LIGHT1);
//	}
//	
//}
//
//void shadingMode (void) {
//	if (isFlat) {
//		glShadeModel(GL_FLAT);
//	} else {
//		glShadeModel(GL_SMOOTH);
//	}
//}



//
//void drawBezierScene(void) {
//	glMatrixMode(GL_MODELVIEW);
//	//Curves
//	glPushMatrix();
//	{
//		for(list<BezierCurve>::iterator it = (*curves_gl).begin(); it != (*curves_gl).end(); it++) {
//			glPushMatrix();
//			{
//				//Matrix Transformation.
//				glMultMatrixf((*it).transMatrix);
//				
//				//Get Curve points
//				int pts = 2*((int) pow(3.0, LOD - 1));
//				vector<Point> curvePts;
//				casteljauCurve((*it), curvePts, pts);
//								
//				//Draw the curve
//				//glLineWidth(1.0);
//				glColor3f(1.0, 0.2, 0.2);
//				glBegin(GL_LINE_STRIP);
//				for(int i = 0; i < curvePts.size(); i++) {
//					glVertex3f(curvePts[i].x, curvePts[i].y, curvePts[i].z);
//				}
//				glEnd();
//			}
//			glPopMatrix();
//		}
//	}
//	glPopMatrix();
//	
//	//Surfaces
//	glPushMatrix();
//	{
//		for(list<BezierSurface>::iterator it = (*surfaces_gl).begin(); it != (*surfaces_gl).end(); it++) {
//			glPushMatrix();
//			{
//				//Matrix Transformation.
//				glMultMatrixf((*it).transMatrix);
//				
//				//Get surface points
//				int pts = 2*((int) pow(3.0, LOD - 1));
//				vector<vector<Point> > surfacePts;
//				casteljauSurface((*it), surfacePts, pts);
//				
//				//Draw the surface
//				//glLineWidth(1.0);
//				glColor3f(1.0, 0.2, 0.2);
//				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//				for(int i = 0; i < surfacePts.size()-1; i++) {
//					glBegin(GL_QUAD_STRIP);
//					for(int j = 0; j < surfacePts.size(); j++) {
//						glVertex3f(surfacePts[i][j].x, surfacePts[i][j].y, surfacePts[i][j].z);
//						glVertex3f(surfacePts[i+1][j].x, surfacePts[i+1][j].y, surfacePts[i+1][j].z);
//					}
//					glEnd();
//				}
//			}
//			glPopMatrix();
//		}
//	}
//	glPopMatrix();
//	glFlush();
//}

void initializeGrid( const int & gridSizeX, const int & gridSizeY )
{
	g_scene_data.gridSizeX = &gridSizeX;
	g_scene_data.gridSizeY = &gridSizeY;
}

void drawGrid(int maxX, int maxY, double gridSize) {

	glPushMatrix();

	glBegin(GL_LINES);
	
	// Grid
	{
		glColor4f( 0.7, 0.7, 0.7, 0.5 );
		for ( double x = - maxX; x <= maxX; x += gridSize ) {
			glVertex2d( x, maxY );
			glVertex2d( x, -maxY );
		}
		
		for ( double y = - maxY; y <= maxY; y += gridSize ) {
			glVertex2d( -maxX, y );
			glVertex2d( maxX, y );
		}
	}
	
	glEnd();
	
	glLineWidth( 2.0 );
	glBegin(GL_LINES);
	
	// Axes
	{
		glColor4f( 0.0, 0.0, 0.0, 0.5 );
		
		glVertex2d( 0, maxY );
		glVertex2d( 0, -maxY );

		glVertex2d( -maxX, 0 );
		glVertex2d( maxX, 0 );
	}
	
	glEnd();
	glLineWidth( 1.0 );
	
	glPopMatrix();
}