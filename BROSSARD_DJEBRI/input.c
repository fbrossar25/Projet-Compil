int main()
{
    int a = 1+1-1+1*2/2;
    int b = 5;
    int c = a * (b - 1 + 1);
    //int e = -d //erreur de syntaxe ';'
    /*
    test commentaire
    sur plusieurs lignes
    */
    //int e = -d; //erreur sémantique sur d
    //int f = 0 / 0; //division par 0
    c++;
    c--;
    c += 1;
    c-=1;

    printf("Hello world !\n");
    printi(42);
    printf("\n"); //caractère spéciaux à ne pas effacer
    printi(c);
    return 0;
}