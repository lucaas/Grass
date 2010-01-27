#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

class Grass
{
private:
    float K; // Fjäderkonstant
    float inertia; // Vridmoment
    float base[3]; // Bas (P0)
    float radius1; // Radie (P0 - P1)
    float theta0; // Jämviktsläge
    float theta; // Nuvarande läge
    float omega; // Vinkelhastighet

public:
    Grass();
    Grass(float x, float z);
    ~Grass();

    void draw();
    void calculate();
};
