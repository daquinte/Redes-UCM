#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

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

      int32_t total = 80* sizeof(char) +2*sizeof(int16_t);
      alloc_data(total);

      char* tmp = _data + sizeof(int32_t);

      memcpy (tmp, name, 80);
      tmp += 80;

      memcpy(tmp, &pos_x, sizeof(int16_t));
      tmp += sizeof(int16_t);
      memcpy(tmp, &pos_y, sizeof(int16_t));
      tmp += sizeof(int16_t);
    }

    int from_bin(char * data)
    {

      char* tmp = data + sizeof(int32_t);
      memcpy(name, tmp, 80);

      memcpy(&pos_x, tmp, sizeof(int16_t));
      tmp += sizeof(int16_t);

      memcpy(&pos_y, tmp, sizeof(int16_t));
      
      tmp += sizeof(int16_t);
      return 0;
    }

   int16_t getPosX () {return pos_x;}
   int16_t getPosY () {return pos_y;}

   char* getName () {return name;}


private:
   
  int16_t pos_x;
  int16_t pos_y;

  static const size_t MAX_NAME = 80;

  char name[MAX_NAME];


};

int main(int argc, char **argv)
{

  Jugador one("player one", 12, 345);
  Jugador one_("-", 0, 0);

  one.to_bin();

  int op = open ("Jugador1", O_RDWR | O_CREAT);

  write(op, one.data(), one.size());

 close(op);

 one_.from_bin(one.data());

 std::cout << "Nombre de one_: " << one_.getName() << std::endl;
 std::cout << "Posicion de one_: " << one_.getPosX() << "," << one_.getPosY() << std::endl;
 

  return 0;


}
