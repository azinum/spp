int _spp_lambda0(int a , int b ) {
return a + b ; 
}
#include <stdlib.h>
#include <stdio.h>
typedef int ( * callback ) ( int , int ) ; void hello ( const char * string , callback cb ) { printf ( "%s: %i\n" , string , cb ( 5 , 6 ) ) ; } int main ( ) { hello ( "Hello there" , _spp_lambda0) ; return 0 ; } 