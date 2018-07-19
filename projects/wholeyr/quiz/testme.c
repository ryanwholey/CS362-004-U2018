#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <math.h>

// This one turned out to be too slow :[
// char inputChar() {
//     int i = floor(rand() % 37);

//     if (i == 0) {
//       return '\0';
//     } else if (i == 36) {
//       return ' ';
//     } else {
//       return (char)(i + 90);
//     }
// }

char validChars[14] = {
  '\0', 
  '[', '(', '{',
  ' ', 'a', 'x',
  '}', ')', ']',
  'r', 'e', 's', 't'
};

char inputChar() {
  return validChars[(int)floor(rand() % 14)];
}

char *inputString() {
  int len = 6;
  char *str = (char*) malloc(len * sizeof(char));
  int i;
  for (i = 0; i < len; i++) {
    str[i] = inputChar();
  }

  return str;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);
    // if (strstr(s, "rese")) {
    //   printf("%s\n", s);
    // }

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' ' && state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      free(s);
      printf("error ");
      exit(200);
    }
    free(s);
  }
}


int main(int argc, char *argv[]) {
  srand(time(NULL));
  testme();
  return 0;
}
