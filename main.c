#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *pf ;
    char buff[255];

    pf = fopen("story.txt", "r");
    fgets(buff, 255, pf); 

    printf("%s", buff);

    fclose(pf);
    return 0;
}

 