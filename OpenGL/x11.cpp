#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <X11/Xlib.h> 

extern int errno; 

int main( void ) {
    Display *d;
    Window w;
    XEvent e;
    char *msg = "Hello, World!";
    int s;
    
    if((d = XOpenDisplay(getenv("DISPLAY"))) == NULL) {
        printf("Can't connect X server: %s\n", strerror(errno));
        exit(1);
    }
    
    s = DefaultScreen( d );
    w = XCreateSimpleWindow( d, RootWindow( d, s ), 10, 10, 200, 200, 1, BlackPixel( d, s ), WhitePixel( d, s ) );
    
    XSelectInput(d, w, ExposureMask|KeyPressMask);
    XMapWindow(d, w);
    
    while(1) {
        XNextEvent(d, &e);
        if(e.type == Expose) {
            XFillRectangle( d, w, DefaultGC( d, s ), 20, 20, 10, 10 );
            XDrawString( d, w, DefaultGC( d, s ), 50, 50, msg, strlen( msg ) ); 
        }
        if(e.type == KeyPress)
          break;
    }
    
    XCloseDisplay( d );
    return 0; 
} 