#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>

#define GLEW_STATIC
#include <GL/glew.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/quaternion.hpp"
#include <AL/al.h>
#include <AL/alc.h>

//--------------------------------------------------\\ SDL files
#include "SDL.h"
#if defined(__WIN32__)
#include "SDL_ttf.h"
#include "SDL_image.h"
#elif defined(__MACOSX__)
#include "SDL_ttf.h"
#include "SDL_image.h"
#endif


#include <btBulletDynamicsCommon.h>


#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

extern "C"{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}
#include "luaBridge/luaBridge.h"
#include "luaBridge/RefCountedPtr.h"

#include <time.h>
#define DegreesToRadians(x) ((x/360)*2*3.14159265358)
#define RadiansToDegrees(x) ((x*360)/(2*3.14159265358))
#define RANDOM_SEEDT() srand(time(NULL))
#define RANDOM_SEED(__AMT__) srand(__AMT__)
#define RANDOM_INT(__MIN__, __MAX__) ((__MIN__) + rand() % ((__MAX__+1) - (__MIN__)))

typedef void (*luaRegister) (lua_State *l);
struct LuaRegistercontainer
{
    lua_State* l;
    std::vector<std::string> Registers;
};

namespace GLOBAL
{
   extern btDiscreteDynamicsWorld *PhysicsWorld;
   extern btDiscreteDynamicsWorld *UIWorld;
   extern float pi;
   extern float Framerate;
   extern float FramerateLock;
   extern int frameCount;
   struct Vertex{
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TextureCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        Vertex(){}
        Vertex(float Posx,float Posy,float Posz,float Normx,float Normy,float Normz,float TexX,float TexY){ //good old TexX
            Position = glm::vec3(Posx,Posy,Posz);
            Normal = glm::vec3(Normx,Normy,Normz);
            TextureCoords = glm::vec2(TexX,TexY);
        }
    };
    struct ObjectCollisionPointer
    {
        void* FirstPointer;
        void* SecondPointer;
    };
    extern Uint32 BULLET_CONTACT_CREATED;
    extern Uint32 BULLET_CONTACT_DESTROYED;

    float Distance(float X1, float X2, float Y1, float Y2,float Z1,float Z2);
    float Clamp(float Value, float Min, float Max);
    float LoopClamp(float Value, float Min, float Max);
    char *textFileRead(std::string filename);
    void Init();
    void startFrame();
    void endFrame();
    bool checkCollision(GLOBAL::ObjectCollisionPointer* col);
    bool checkCollision(void *a, void *b);

    LuaRegistercontainer* getContainer(lua_State *l);
    bool isRegistered(std::string TypeID, lua_State *l);
    void addRegister(std::string TypeID, lua_State *l);

    void RegisterLua(lua_State *l);
}
extern std::vector<LuaRegistercontainer*> statereg;
extern std::vector<GLOBAL::ObjectCollisionPointer>Collisions;
#endif // GLOBAL_H_INCLUDED
