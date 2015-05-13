//通过OpenGL绘图进行加速


BOOL bSetupPixelFormat(HDC hdc);

GLvoid resize(GLsizei width, GLsizei height);

GLvoid createObjects();

GLvoid initializeGL(GLsizei width, GLsizei height);

void polarView(GLdouble radius, GLdouble twist, GLdouble latitude,
	GLdouble longitude);

GLvoid drawScene(GLvoid);