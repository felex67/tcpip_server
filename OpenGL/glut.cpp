/*http://esate.ru, isaer*/
//подключаем библиотеку glut 
#include <GL/glut.h> 
void dis() //вот и та функция дисплея 
{
    glClear(GL_COLOR_BUFFER_BIT);
    //очищаем буфер цвета
    glPointSize(5);
    //устанавливаем размер точки (так как если не установим, то она будет очень маленькой)
    glBegin(GL_POINTS);
    //начинаем рисовать точку (также есть и другие флаги как GL_LINES GL_TRAINGLE (вроде так =)) ) если вы рисуете линии то вам надо указать 2 точки и нарисуются линии
    glColor3f(0.0, 0.0, 0.0);
    // устанавливаем цвет которым будем рисовать (если вы рисуете много точек или линий то можно пере тем как нарисовать что- либо установить для него цвет, если вам не устраивает цвет то 0 до 1 то можно установить glColor3ub и тогда будут 255 цвета
    glVertex2f(0.5, 0.5);
    //рисуем точку 2f означает, что мы будем рисовать в 2 координатах и там координаты 
    //(а теперь можно пояснить glOrtho если бы мы установили размер экрана 100х100 то и поставили в glOrtho 
    //значение glOrtho(0.0, 100, 0.0, 100, -1.0, 1.0); то эту точку можно было бы нарисовать так glVertex2f(25, 25);
    //и она была бы там же (да и если мы написали бы вместо 100 480 она была бы там ) надеюсь разобрались
    //если вам надо много точек то не надо писать заново все с glBegin и тд можно просто написать сюда множество 
    //точек и они нарисуются 
    glEnd();
    //заканчиваем рисовать 
    glFlush();
    //отрисовываем все что нарисовали 
}

int main(int argc, char **argv) //точка входа приложения
{
    glutInit(&argc, argv);
    //инициализация glut 
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
    //установка дисплея GLUT_SINGLE означает что мы будем использовать 1 буфер (для 2D графики) а GLUT_RGB означает 
    //что мы будем использовать RGB цвета 
    glutInitWindowSize(480, 480);   //устанавливаем размер окна glutInitWindowPosition(200, 200);
    //устанавливаем положение окна при запуске
    glutInitWindowPosition(400, 100);
    glutCreateWindow("First programm"); //создаем окно с названием "First program"
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //очищаем окно в белый цвет 
    glMatrixMode(GL_PROJECTION);    //устанавливаем матрицу проекции 
    glLoadIdentity();   //сбрасываем матрицу (устанавливаем нулевую) 
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); //устанавливаем ортогональный вид (или как он там называется в скобках значения)
    // которым будет все измеряться то есть первые 2 - это по Х, вторые 2 - это про У и третьи 2 - это по Z. для более 
    //подробного понятия значения от нуля до единицы по Х это будет само окно, то есть 480 (не знаю как объяснить) 
    //значение Z не имеет значения так как мы рисуем 2D 
    glutDisplayFunc(dis);//указываем функцию дисплея(ну или рисования) 
    glutMainLoop();//цикл обработки всякого (типа нажатия на клавишу и тд)
    return 0;
}
//Подробнее на esate.ru: http://esate.ru/blogs/isaer/4073/