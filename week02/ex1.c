#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
   int intMax;
   unsigned short int unshortintMax;
   signed long int silongintMax;
   float floatMax;
   double doubleMax;
   
   intMax = INT_MAX;
   unshortintMax = USHRT_MAX;
   silongintMax = LONG_MAX;
   floatMax = FLT_MAX;
   doubleMax = DBL_MAX;
   
   printf("The size of INT = %lu\n", sizeof(intMax));
   printf("The size of UNSIGNED SHORT INT = %lu\n", sizeof(unshortintMax));
   printf("The size of SIGNED LONG INT = %lu\n", sizeof(silongintMax));
   printf("The size of FLOAT = %lu\n", sizeof(floatMax));
   printf("The size of DOUBLE = %lu\n", sizeof(doubleMax));
   
   printf("The maximum value of INT = %d\n", intMax);
   printf("The maximum value of UNSIGNED SHORT INT = %d\n", unshortintMax);
   printf("The maximum value of SIGNED LONG INT = %ld\n", silongintMax);
   printf("The maximum value of FLOAT = %f\n", floatMax);
   printf("The maximum value of DOUBLE = %f\n", doubleMax);
   
   return 0;
   
}
