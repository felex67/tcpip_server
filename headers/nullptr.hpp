#ifndef _nullptr_hpp_
#define _nullptr_hpp_
      #ifndef __cplusplus11

        class nullptr {
        public:
            operator void*() const {
                return 0;
            }
        };

    #endif //__cplusplus11
#endif //_nullptr_hpp_