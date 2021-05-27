#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader/shader_m.h"
using namespace std;
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
float* readPointCloud(string name);
void pcdToXyz(string infilePath, string outfilePath);
void pcdToXyztgb(string infilePath, string outfilePath);
void pcdToImformation(string infilePath, string outfilepath);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int verticeNum = 0;
static float size = 1;
static float trans_x = 0, trans_y = 0, trans_z = 0;
static string pcdName ="C:\\Users\\ccyys\\CLionProjects\\LearnOpenGL\\asset\\data\\point_cloud\\kk.pcd";
static string txtName = "C:\\Users\\ccyys\\CLionProjects\\LearnOpenGL\\asset\\data\\point_cloud\\rops_cloud.txt";
float angleZ = 0, angleX = 0, angleY = 0;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float colorR = 0.5f;
float colorG = 0.5f;
float colorB = 0.5f;
float fov = 45.0f;
float maxX = -1.0f, maxY = -1.0f, maxZ = -1.0f;
float minX = 1.0f, minY = 1.0f, minZ = 1.0f;
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
    glfwSetCursorPosCallback(window, mouse_callback);

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


    for(int i = 0;i < verticeNum;i++){
        if(i%3==0){
            if(vertices[i] > maxX){
                maxX = vertices[i];
            }
            if(vertices[i] < minX){
                minX = vertices[i];
            }
        }else if(i%3==1){
            if(vertices[i] > maxY){
                maxY = vertices[i];
            }
            if(vertices[i] < minY){
                minY = vertices[i];
            }
        }else{
            if(vertices[i] > maxZ){
                maxZ = vertices[i];
            }
            if(vertices[i] < minZ){
                minZ = vertices[i];
            }
        }
    }
    float redBox[] = {
            minX,maxY,minZ,
            minX,maxY,maxZ,
            maxX,maxY,maxZ,
            minX,maxY,maxZ,
            maxX,maxY,maxZ,
            maxX,maxY,minZ,
            minX,maxY,minZ,
            maxX,maxY,minZ, //up 4
            minX,minY,minZ,
            minX,minY,maxZ,
            maxX,minY,maxZ,
            minX,minY,maxZ,
            maxX,minY,maxZ,
            maxX,minY,minZ,
            minX,minY,minZ,
            maxX,minY,minZ, //down 4
            minX,minY,minZ,
            minX,maxY,minZ,
            minX,minY,maxZ,
            minX,maxY,maxZ,
            maxX,minY,minZ,
            maxX,maxY,minZ,
            maxX,minY,maxZ,
            maxX,maxY,maxZ //middle 4
    };

    cout << maxX << " " << maxY << " " << maxZ << endl;
    cout << minX << " " << minY << " " << minZ << endl;



    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    //first one
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, verticeNum*4, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //second one
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24 * 3, redBox, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // -------------------------------------------------------------------------------------------
    ourShader.use();



    // render loop
    // -----------
    float mouseX=0, moustY=0;
    //set camera


    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f,1.0f,1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

        glfwSetScrollCallback(window,scroll_callback);


        transform = glm::rotate(transform, angleZ, glm::vec3(0.0f,0.0f,1.0f));
        transform = glm::rotate(transform, angleX, glm::vec3(1.0f,0.0f,0.0f));
        transform = glm::rotate(transform, angleY, glm::vec3(0.0f,1.0f,0.0f));
        transform = glm::scale(transform, glm::vec3(size,size,size));
        transform = glm::translate(transform,glm::vec3(trans_x, trans_y, trans_z));
        glm::vec3 color = glm::vec3(colorR,colorG, colorB);
        ourShader.setVec3("ourColor",color);




        // get matrix's uniform location and set matrix
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render container
        glBindVertexArray(VAO[0]);
        glPointSize(5.0);
        glDrawArrays(GL_POINTS, 0, verticeNum/3);

        //draw box
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_LINES, 0, 24);
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);

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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        angleX+=0.001f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        angleX-=0.001f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        angleY+=0.001f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        angleY-=0.001f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        trans_y+=0.0005f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        trans_y-=0.0005f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        trans_x-=0.0005f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        trans_x+=0.0005f;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        colorR=1.0f;
        colorG=0.0f;
        colorB=0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        colorR=0.0f;
        colorG=1.0f;
        colorB=0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        colorR=0.0f;
        colorG=0.0f;
        colorB=1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
        colorR=1.0f;
        colorG=0.0f;
        colorB=1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS){
        colorR=0.0f;
        colorG=1.0f;
        colorB=1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS){
        colorR=1.0f;
        colorG=1.0f;
        colorB=0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS){
        colorR=0.5f;
        colorG=0.5f;
        colorB=0.5f;
    }


    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        angleX = 0;
        angleY = 0;
        angleZ = 0;
        trans_x = 0;
        trans_y = 0;
        trans_z = 0;
    }
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top


        angleX -= 0.005*yoffset;
        angleY -= 0.005*xoffset;

    }
    lastX = xpos;
    lastY = ypos;


}
