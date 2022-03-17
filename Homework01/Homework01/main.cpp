//
//  main.cpp
//  Homework01
//
#define GL_SILENCE_DEPRECATION
//GLFW: window, 및 이벤트 관련 처리 프레임워크
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// window size
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        std::cout << "Failed to initiallize GLFW" << std::endl;
        return -1;
    };

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "01_1_Immediate Mode", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // glfwMakeContextCurrent : makes the OpenGL context of the specified window current on the calling thread. A context must only be made current on a single thread at a time and each thread can have only a single current context at a time.
    // ref: https://www.glfw.org/docs/3.3/group__context.html
    // Q: what is 'context'?
    glfwMakeContextCurrent(window);
    
    // glfwSetFramebufferSizeCallback: sets the framebuffer resize callback of the specified window, which is called when the framebuffer of the specified window is resized.
    // ref: https://www.glfw.org/docs/3.0/group__window.html#ga3203461a5303bf289f2e05f854b2f7cf
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    //glfwSetKeyCallback:This function sets the key callback of the specified window, which is called when a key is pressed, repeated or released.
    // ref: https://www.glfw.org/docs/3.3/group__input.html
    glfwSetKeyCallback(window, key_callback);

    // glViewport: set viewport inside the window.
    // ref: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glViewport.xhtml
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    // glClearColor: set clear values when glClear(GL_COLOR_BUFFER_BIT); is called.
    // ref: https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glClearColor.xml
    // Q: why opacity not working?
    glClearColor(1.f, 1.f, 1.f, 1.0f);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        //Clear out indicated buffer
        //ref: https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glClear.xml
        glClear(GL_COLOR_BUFFER_BIT);
        
        // glColor3f: set current color in floats
        // ref: https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColor.xml
        glColor3f(0.f, 1.f, 0.f);
        
        // OpenGL matrix related
        //ref: http://www.songho.ca/opengl/gl_transform.html
        
        //glMatrixMode: specify which matrix to handle.
        glMatrixMode(GL_PROJECTION);
        //glLoadIdentity: initalize the target matrix to identity matrix.
        glLoadIdentity();
        float ratio = SCR_WIDTH / (float)SCR_HEIGHT;
        // glOrtho: setting orthogonal projection frustum with arguments.
        // ref: https://skyfe.tistory.com/entry/iOS-OpenGL-ES-%ED%8A%9C%ED%86%A0%EB%A6%AC%EC%96%BC-4%ED%8E%B8
        glOrtho(-ratio, ratio, -1.f, 1.f, -1.f, 1.f);
        glMatrixMode(GL_MODELVIEW);

        // glBegin: specify how vertexs are interpreted. There are 10 ways of interpretation.
        // ref: https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml
        glBegin(GL_TRIANGLES);

            // glVertex3f: specify a vertex in float.
            // ref: https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertex.xml
            glVertex3f(-0.5f, -0.5f, 0.f);
            glVertex3f(-0.5f, 0.5f, 0.f);
            glVertex3f(0.5f, -0.5f, 0.f);

        // glEnd: Used with glBegin, close glBegin.
        glEnd();

        
        glBegin(GL_TRIANGLES);
            glVertex3f(0.5f, -0.5f, 0.f);
            glVertex3f(0.5f, 0.5f, 0.f);
            glVertex3f(-0.5f, 0.5f, 0.f);
        glEnd();

        // glfwSwapBuffers: swaps the front and back buffers of the specified window.
        // ref: https://www.glfw.org/docs/3.0/group__context.html
        glfwSwapBuffers(window);
        
        // ref: https://www.glfw.org/docs/3.3/group__window.html
        // I don't really understand its description.
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}
