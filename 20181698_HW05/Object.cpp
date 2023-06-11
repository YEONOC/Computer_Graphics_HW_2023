#include "Object.h" 
void Object::init_buffer_objects() 
{   
    //TODO
    std::vector<float> vertices;
    std::vector<float> colors;
    
    for (unsigned int i = 0; i < pmesh_->mNumVertices; ++i)
    {
        aiVector3D vertex = pmesh_->mVertices[i];
        //std::cout<< "  vertex  (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
	vertices.push_back(vertex.x);
	vertices.push_back(vertex.y);
	vertices.push_back(vertex.z);
	
        if (pmesh_->mColors[0] != NULL)
        {
            aiColor4D color = pmesh_->mColors[0][i];
            //std::cout<<"  color  (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")" << std::endl;
            colors.push_back(color.r);
            colors.push_back(color.g);
            colors.push_back(color.b);
        }
    }
    
    for(unsigned int i = 0; i < pmesh_->mNumFaces; i++){
        aiFace face = pmesh_ -> mFaces[i];
        for(unsigned int j = 0; j< face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    // VBO
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors)*colors.size(), &colors[0], GL_STATIC_DRAW);

    // IBO 
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*indices.size(), &indices[0], GL_STATIC_DRAW);
}

glm::mat4 Object::get_model_matrix() const
{
    glm::mat4 mat_model = glm::mat4(1.0f);
    
    // TODO
    mat_model = mat_model * glm::translate(glm::mat4(1.0f), vec_translate_);
    mat_model *= glm::mat4_cast(quat_rotate_);
    mat_model = mat_model * glm::scale(glm::mat4(1.0f), vec_scale_);

    return mat_model;
}

void Object::draw(int loc_a_position, int loc_a_color)
{
    // TODO
    // 앞으로 언급하는 배열 버퍼(GL_ARRAY_BUFFER)는 position_buffer로 지정
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    // 버텍스 쉐이더의 attribute 중 a_position 부분 활성화
    glEnableVertexAttribArray(loc_a_position);
    // 현재 배열 버퍼에 있는 데이터를 버텍스 쉐이더 a_position에 해당하는 attribute와 연결
    glVertexAttribPointer(loc_a_position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 앞으로 언급하는 배열 버퍼(GL_ARRAY_BUFFER)는 color_buffer로 지정
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    // 버텍스 쉐이더의 attribute 중 a_color 부분 활성화
    glEnableVertexAttribArray(loc_a_color);
    // 현재 배열 버퍼에 있는 데이터를 버텍스 쉐이더 a_color에 해당하는 attribute와 연결
    glVertexAttribPointer(loc_a_color, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // IBO를 이용해 물체 그리기
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    
    // 정점 attribute 배열 비활성화
    glDisableVertexAttribArray(loc_a_position);
    glDisableVertexAttribArray(loc_a_color);
}
    
void Object::print_info()
{
    std::cout << "print mesh info" << std::endl;

    std::cout << "num vertices " << pmesh_->mNumVertices << std::endl;
    for (unsigned int i = 0; i < pmesh_->mNumVertices; ++i)
    {
        aiVector3D vertex = pmesh_->mVertices[i];
        std::cout<< "  vertex  (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;

        if (pmesh_->mColors[0] != NULL)
        {
            aiColor4D color = pmesh_->mColors[0][i];
            std::cout<<"  color  (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")" << std::endl;
        }
    }
}
