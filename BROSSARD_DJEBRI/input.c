int main()
{
    int a;
    a = 1+1-(1+1)*2/2; // = 0
    printi(a); //0
    printf("\n");
    int b = 5;
    int c = a * (b + 1 + 1) + 1; // = 1
    //int e = -d //erreur de syntaxe ';'
    /*
    test commentaire
    sur plusieurs lignes
    */
    //int e = -d; //erreur sémantique sur d
    //int f = 0 / 0; //division par 0
    c++;
    printi(c); // 2
    c--;
    c += 1;
    c-=1;

    printf("\nHello world !\n");
    printi(42);
    printf("\n"); //caractère spéciaux à ne pas effacer
    printi(c); // 1
    return 0;
}
