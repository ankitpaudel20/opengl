#include <string>
#include "core.h"
#include "Texture.h"
#include "Vertexarray.h"
#include "buffer.h"
#include "Drawable.h"
#include "Shader.h"

class Mesh :public Drawable<Vertex2>{
protected:
	virtual void preDraw(Shader& shader) {
        shader.Bind();
        matModel = translation * rotation * scaling;
        shader.SetUniform<glm::mat4*>("model", &matModel);

        m_vao.Bind();
        // m_ibo.Bind();
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        //for (unsigned int i = 0; i < m_tex.size(); i++)
        //{
        //    // retrieve texture number (the N in diffuse_textureN)
        //    std::string number;
        //    std::string name = m_tex[i].type;
        //    if (name == "texture_diffuse")
        //        number = std::to_string(diffuseNr++);
        //    else if (name == "texture_specular")
        //        number = std::to_string(specularNr++);
        //    shader.SetUniform<float>(("material." + name + number).c_str(), i);
        //    m_tex[i].Bind(i);  // bind in ith slot
        //}

        for (int i = 0; i < m_tex.size(); i++)
        {
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = m_tex[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.SetUniform<int>(("material." + name ).c_str(), i);
            m_tex[i].Bind(i);  // bind in ith slot
        }
        glActiveTexture(GL_TEXTURE0);
	}
public:
    Mesh(const std::vector<Vertex2>& vertices, const std::vector<unsigned>& indices, const std::vector<Texture>& tex ) :Drawable(vertices, indices, tex) {}
};