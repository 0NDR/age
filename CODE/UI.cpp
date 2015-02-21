#include "UI.h"
#include <typeinfo>

void UI::setColor(glm::vec4 pos){Color = pos;}

glm::vec4 *UI::getColor(){return &Color;}

void UI::setMesh(Mesh* newMesh)
{
    renderMesh = newMesh;
}
void UI::Update()
{

}
void UI::Render()
{
        if(renderShader == NULL)
        {
            std::cout<<type()<<" "<<Name<<" is missing a shader"<<std::endl;
            return;
        }
        renderShader->Activate();
        glDisable(GL_DEPTH_TEST);
        GLuint ShaderProgram = renderShader->ShaderProgram;
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "model" ),1,GL_FALSE,glm::value_ptr(getAbsoluteMatrix()));

        GLint unioverColor = glGetUniformLocation(ShaderProgram,"ObjectColor");
        glUniform4f(unioverColor,Color.x,Color.y,Color.z,Color.w);
        renderMesh->drawToShader(renderShader);
        glEnable(GL_DEPTH_TEST);
}
