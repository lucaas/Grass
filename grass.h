#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

class Grass
{
private:
    float base[3];
    float mcolor[4];

public:
    Grass();
    ~Grass();

    void draw();
};
