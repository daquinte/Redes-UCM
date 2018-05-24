#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):pos_x(_x),pos_y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
    }

    int from_bin(char * data)
    {
    }

  int16 getPosX () {return pos_x;}
  int16 getPosY () {return pos_y;}

  char* getName () {return name;}


private:
   
  int16_t pos_x;
  int16_t pos_y;

  char name[MAX_NAME];
  static const size_t MAX_NAME = 20;

};

int main(int argc, char **argv)
{

  Jugador* player = new Jugador("player one", 12, 345);

  std::fstream fs;
  fs.open ("Jugador1.txt", /*No sé qué poner aquí.com*/);


}
