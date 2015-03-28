#include "GLOBAL.h"
#include "CombinedController.h"
#include "GameShader.h"
#include "gameObject.h"
#include "Light.h"
#include "ResourceFactory.h"
#include "Model.h"
#include "BonedModel.h"
bool locked = false,warped = false;
glm::vec3 look,position;
CombinedController *CombCont;
int normOn = 1;
void key(SDL_Event e)
{

    switch(e.key.keysym.sym)
    {

        case SDLK_i:
            position.x-=sin(GLOBAL::pi-DegreesToRadians(look.y))*fabs(cos(DegreesToRadians(look.x)));
            position.z-=cos(GLOBAL::pi-DegreesToRadians(look.y))*fabs(cos(DegreesToRadians(look.x)));
            position.y+=sin(DegreesToRadians(look.x));
            break;
        case SDLK_k:
            position.x+=sin(GLOBAL::pi-DegreesToRadians(look.y))*fabs(cos(DegreesToRadians(look.x)));
            position.z+=cos(GLOBAL::pi-DegreesToRadians(look.y))*fabs(cos(DegreesToRadians(look.x)));
            position.y-=sin(DegreesToRadians(look.x));
            break;
        case SDLK_l:
            position.x-=cos(DegreesToRadians(look.y));
            position.z-=sin(DegreesToRadians(look.y));
            break;
        case SDLK_j:
            position.x+=cos(DegreesToRadians(look.y));
            position.z+=sin(DegreesToRadians(look.y));
            break;
        case SDLK_SPACE:
            locked=!locked;
            break;
        case SDLK_q:
            normOn = normOn==1?0:1;
            break;
    }

}
void mouseMoved(SDL_Event e)
{
    if(locked)
    {
        if(!warped)
        {
            look.y = look.y +((float)e.motion.xrel)/10.0;
            look.x = look.x +((float)e.motion.yrel)/10.0;
            if (look.y>=360){look.y =0;}
            if (look.y<0){look.y = 360;}
                look.x = GLOBAL::Clamp(look.x, -90, 90);
        }
        else
        {
            warped = false;
        }
        if (GLOBAL::Distance(e.motion.x, CombCont->getSize().x/2, e.motion.y, CombCont->getSize().y/2, 0, 0)>150)
        {
            warped = true;
            SDL_WarpMouseInWindow(CombCont->getWindow(),CombCont->getSize().x/2, CombCont->getSize().y/2);
        }
    }
}


void resizeViewport(SDL_Event e)
{

    if(e.window.event==SDL_WINDOWEVENT_RESIZED)
        glViewport(0,0,e.window.data1,e.window.data2);
}
int main(int argc, char *argv[])
{
    GLOBAL::Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    CombCont = new CombinedController(100,100,512,512,SDL_WINDOW_OPENGL| SDL_WINDOW_RESIZABLE);

    CombCont->addEvent(key,SDL_KEYDOWN);
    CombCont->addEvent(mouseMoved,SDL_MOUSEMOTION);
    CombCont->addEvent(resizeViewport,SDL_WINDOWEVENT);

    GameShader RenderShader, NormalsShader;
    ResourceFactory RF;
    BonedModel* renderModel;
    Model *PlaneModel;
    gameObject renderObject, PlaneObject;
    Light renderLight, skylight;

    RenderShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Multishader.glsl");
    RenderShader.LinkProgram();
    NormalsShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/NormalsShader.glsl");
    NormalsShader.LinkProgram();
    renderModel = RF.loadFromFile<BonedModel>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/boblamp/boblampclean.md5mesh");
    PlaneModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/MarblePlane/marbleplane.obj");

    renderObject.setMesh(renderModel);
    renderObject.setScale(glm::vec3(1,1,1));
    renderObject.setColor(glm::vec4(1,1,1,1));
    PlaneObject.setMesh(PlaneModel);
    PlaneObject.setScale(glm::vec3(1,1,1));
    PlaneObject.setColor(glm::vec4(1,1,1,1));

    renderLight.DiffuseColor = glm::vec4(255,204,153,255)*3/255;
    renderLight.AmbientColor = glm::vec4(0,0,0,1);
    renderLight.SpecularColor = glm::vec4(255,204,153,255)*5/255;
    renderLight.Position = glm::vec3(0,2,0);
    renderLight.Direction = glm::vec3(0,0,0);
    renderLight.LinearAttenuation = 0.1;

    skylight.DiffuseColor = glm::vec4(255,255,255,255)*2/255;
    skylight.AmbientColor = glm::vec4(0,0,0,1);
    skylight.SpecularColor = glm::vec4(255,255,255,255)*9/255;
    skylight.Position = glm::vec3(0,25,0);
    skylight.Direction = glm::vec3(0,0,0);
    skylight.LinearAttenuation = .1;
    Uint32 sTime = SDL_GetTicks();
    while(true)
    {
        CombCont->CheckKeys();
        Lights.clear();

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0,1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
        glEnable(GL_DEPTH_TEST);


        // -------RenderShader
        RenderShader.setProjectionMatrix(75,CombCont->getAspectRatio(),0.01,1000);
        RenderShader.setViewMatrix(position,look);
        glUniform1i(glGetUniformLocation(RenderShader.ShaderProgram,"TextureOn"),1);
        glUniform1i(glGetUniformLocation(RenderShader.ShaderProgram,"isCube"),0);
        glUniform1i(glGetUniformLocation(RenderShader.ShaderProgram,"LightsOn"),1);
        glUniform1i(glGetUniformLocation(RenderShader.ShaderProgram,"NormalOn"),normOn);
        glUniform3f(glGetUniformLocation(RenderShader.ShaderProgram, "viewpos"),-position.x,-position.y,-position.z);
        float runTime = (float)((double)SDL_GetTicks()-(double)sTime)/1000.f;

        glm::vec3 lPos = -position;//glm::vec3((renderModel->getTransform("lamp"))*glm::vec4(-39.749374,-6.182379 ,40.334176 ,1));
        renderLight.setPosition(lPos);
        renderLight.Render(&RenderShader);
        skylight.Render(&RenderShader);
        PlaneObject.setPosition(glm::vec3(0,0,0));
        PlaneObject.setTextureScale(glm::vec2(100.f,100.f));
        PlaneObject.setScale(1000,1,1000);
        PlaneObject.Render(&RenderShader);
        CombCont->Swap();
    }
    return 0;
}

