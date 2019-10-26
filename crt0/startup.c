extern int main(int, char *[]);

void startup()
{
    int argc = 1;
    char *argv[1] = { "Prog" };
    int ret = main(argc, argv);
}