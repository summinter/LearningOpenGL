#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
using namespace std;
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float* readPointCloud(string name);
void pcdToXyz(string infilePath, string outfilePath);
void pcdToXyztgb(string infilePath, string outfilePath);
void pcdToImformation(string infilePath, string outfilepath);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
int verticeNum = 0;
static float size = 1;
static float trans_x = 0, trans_y = 0, trans_z = 0;
static string pcdName ="C:\\Users\\ccyys\\CLionProjects\\LearnOpenGL\\asset\\data\\point_cloud\\kk.pcd";
static string txtName = "C:\\Users\\ccyys\\CLionProjects\\LearnOpenGL\\asset\\data\\point_cloud\\rops_cloud.txt";
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("C:\\Users\\ccyys\\CLionProjects\\LearnOpenGL\\vs\\5.1.transform.vs", "C:\\Users\\ccyys\\CLionProjects\\LearnOpenGL\\fs\\5.1.transform.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float *vertices = readPointCloud(txtName);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticeNum*4, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("C:\\Users\\ccyys\\CLionProjects\\LearnOpenGL\\asset\\image\\container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("C:\\Users\\ccyys\\CLionProjects\\LearnOpenGL\\asset\\image\\awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


    // render loop
    // -----------
    float angleZ = 0, angleX = 0, angleY = 0;
    float mouseX=0, moustY=0;
    //set camera


    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        float x,y,z;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            angleZ+=0.001f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            angleZ-=0.001f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            angleX+=0.001f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            angleX-=0.001f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            angleY+=0.001f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            angleY-=0.001f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            trans_y+=0.0005f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            trans_y-=0.0005f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            trans_x-=0.0005f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            trans_x+=0.0005f;

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            angleX = 0;
            angleY = 0;
            angleZ = 0;
            trans_x = 0;
            trans_y = 0;
            trans_z = 0;
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
            if(size < 5){
                size += 0.001;
            }else{
                size -= 0.001;
            }
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)){
            if(size > 0.1){
                size -= 0.001;
            }else{
                size += 0.001;
            }
        }
        glfwSetScrollCallback(window,scroll_callback);


        transform = glm::rotate(transform, angleZ, glm::vec3(0.0f,0.0f,1.0f));
        transform = glm::rotate(transform, angleX, glm::vec3(1.0f,0.0f,0.0f));
        transform = glm::rotate(transform, angleY, glm::vec3(0.0f,1.0f,0.0f));
        transform = glm::scale(transform, glm::vec3(size,size,size));
        transform = glm::translate(transform,glm::vec3(trans_x, trans_y, trans_z));

        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        float radius = 10.0f;
        float camX   = sin(glfwGetTime()) * radius;
        float camZ   = cos(glfwGetTime()) * radius;

        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.setMat4("view", view);


        // get matrix's uniform location and set matrix
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render container
        glBindVertexArray(VAO);
        glPointSize(5.0);
        glDrawArrays(GL_POINTS, 0, verticeNum/3);
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (size >= 0.1f && size <= 5.0f) {
        size -= 0.1 * yoffset;
    }

    size = size <= 0.1f ? 0.1f : size;
    size = size >= 5.0f ? 5.0f : size;
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
float* readPointCloud(string name){
    float arr[100000]={0};
    verticeNum = 0;
    ifstream infile(name);
    while(!infile.eof())
    {   float temp;
        infile>>temp;
        arr[verticeNum] = temp;
        ++verticeNum;
    }
    infile.close();
    float* points = new float[verticeNum];
    for ( int j= 0; j < verticeNum; ++j){
        points[j] = arr[j];
    }
    return points;
}
void pcdToXyz(string infilePath, string outfilePath) {
    ifstream infile(infilePath);
    ofstream outfile(outfilePath);
    string x;
    string y;
    string z;
    string temp;
    if (!infile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    for (int i = 0;i < 11;i++) {
        getline(infile, temp);
    }
    while (getline(infile, temp))
    {
        infile >> x >> y >> z;
        infile >> temp;
        outfile << x << "\t" << y << "\t" << z << endl;
    }
    infile.close();
    outfile.close();

}


void pcdToXyztgb(string infilePath, string outfilePath) {
    ifstream infile(infilePath);
    ofstream outfile(outfilePath);
    string x;
    string y;
    string z;
    string rgb;
    string temp;
    if (!infile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    for (int i = 0;i < 11;i++) {
        getline(infile, temp);
    }
    while (getline(infile, temp))
    {
        infile >> x >> y >> z>>rgb;

        outfile << x << "\t" << y << "\t" << z <<rgb<< "\t" <<endl;
    }
    infile.close();
    outfile.close();

}


void pcdToImformation(string infilePath, string outfilepath) {
    ifstream infile(infilePath);
    ofstream outfile(outfilepath);
    string x;
    string y;
    string z;
    string rgb;
    string temp;
    if (!infile.is_open())
    {
        cout << "未成功打开文件" << endl;
    }
    for (int i = 0;i < 11;i++) {
        getline(infile, temp);
    }

    infile.close();
    outfile.close();

}