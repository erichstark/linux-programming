#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

//typ reprezentujuci hodnotu prepinaca (zadany, nezadany)
typedef enum {
    UNSET = 0,
    SET = 1
} OPTION;

//struktura reprezentujuca vstupne argumenty
typedef struct {

    OPTION h; //volba h (help) bez argumentu
    OPTION a; //volba a bez argumentu
    OPTION b; //volba b s povinnym argumentom
    char*  bArg; //argument volby b
    OPTION c; //volba c s povinnym argumentom
    char*  cArg;    //argument volby c ako retazec (nastavi sa pri parsovani)
    int    cIntArg; //argument volby c ako cislo   (nastavi sa pri validovani)
    OPTION d; //volba d s volitelnym argumentom
    char*  dArg; //argument volby d
} ARGS;

//vypis help-u a ukoncenie programu
void prinHelpAndExit(FILE* stream, int exitCode){

    fprintf(stream, "Usage: parametre [-h] [-a | alpha] [(-b | --beta) argB] (-c | --gamma) argC [(-d | delta) [argD]]\n");
    fprintf(stream, "Program vypise nastavenie svojich vstupnych argumentov\n");
    fprintf(stream, "Prepinace:\n");
    fprintf(stream, " -h, --help   vypise help\n");
    fprintf(stream, " -a, --alpha  popis vyznamu parametra .....\n");
    fprintf(stream, " -b, --beta   popis vyznamu parametra .....\n");
    fprintf(stream, " -c, --gamma  popis vyznamu parametra .....\n");
    fprintf(stream, " -d, --delta  popis vyznamu parametra .....\n");
    exit(exitCode);
}

//parsovanie argumentov
void parseArgs(int argc, char * argv[], ARGS * args) {

    int opt;

    struct option longOptions[] = {

        {
            "help",  no_argument,       NULL, 'h'},
        {
            "alpha", no_argument,       NULL, 'a'},
        {
            "beta",  required_argument, NULL, 'b'},
        {
            "gamma", required_argument, NULL, 'c'},
        {
            "delta", optional_argument, NULL, 'd'},
        {
            NULL,    0,                 NULL, 0  }                              
    };

    //inicializacia parametrov na defaultne hodnoty
    args->h = UNSET;
    args->a = UNSET;
    args->b = UNSET;
    args->bArg = NULL;
    args->c = UNSET;
    args->cArg = NULL;
    args->d = UNSET;
    args->dArg = NULL;

    //parsovanie argumentov
    printf("Debug: parsovanie argumentov:\n");
    do {

        //opt = getopt(argc, argv, ":hab:c:d::");
        //int optionIndex = 0;
        opt = getopt_long(argc, argv, ":hab:c:d::", longOptions, NULL /*&optionIndex*/);
        printf("Debug:    opt = '%c'(%3d), optopt='%c', optarg=%s, optind=%d, opterr=%d\n",
                opt, opt, optopt, optarg, optind, opterr);
        //printf("Debug:  %s %s \n", longOptions[optionIndex].name, argv[optind-1]);
        switch(opt) {

            case 'h': 
                args->h = SET;
                break;
            case 'a':
                args->a = SET;
                break;
            case 'b':
                args->b = SET;
                args->bArg = optarg;
                break;
            case 'c': 
                args->c = SET;
                args->cArg = optarg;
                break;
            case 'd':
                args->d = SET;
                args->dArg = optarg;
                break;
            case ':':
                fprintf(stderr, "CHYBA: nezadany argument volby '%c'\n", optopt);
                prinHelpAndExit(stderr, EXIT_FAILURE);
                break;
            case '?':
                //pri pouziti getopt_long nie je v manualy uvedene ako zistit neznamu dlhu volbu
                //preto vyuzitie indexu 'optind' (presnejsie argv[optind-1]) nemusi vzdy fungovat
                fprintf(stderr, "CHYBA: neznama volba '%c'\n", optopt);
                prinHelpAndExit(stderr, EXIT_FAILURE);
                break;
        }
    }while(opt != -1);

    printf("Debug: zvysne argumenty:\n");
    while(optind < argc ) {

        printf("Debug:    non-option ARGV-element: %s\n", argv[optind++]);
    }
}

//validacia argumentov
void validateArgs(ARGS * args) {


    //ak nebol zadany argument prepinaca d, tak nastav jeho hodnotu na prazdny retazec
    if( args->dArg == NULL ) {

        args->dArg = "nenastaveny";
    }

    //over ci bol zadany povinny prepinac c
    if( args->c == UNSET ) {

        fprintf(stderr, "CHYBA: nie je zadany povinna volba 'c'\n");
        prinHelpAndExit(stderr, EXIT_FAILURE);
    }

    //skonvertuj argument prepinaca c na cislo
    if( sscanf(args->cArg, "%d", &args->cIntArg) != 1 ) {

        fprintf(stderr, "CHYBA: argument volby c musi byt cislo\n");
        prinHelpAndExit(stderr, EXIT_FAILURE);
    }
}

//debug vypis zadanych argumentov
void printOptions(ARGS * args) {

    printf("vypis parametrov programu:\n");
    printf("    h: %s\n", args->h==SET?"SET":"UNSET");
    printf("    a: %s\n", args->a==SET?"SET":"UNSET");
    printf("    b: %s arg: %s\n", args->b==SET?"SET":"UNSET", args->bArg);
    printf("    c: %s arg: %s\n", args->c==SET?"SET":"UNSET", args->cArg);
    printf("    d: %s arg: %s\n", args->d==SET?"SET":"UNSET", args->dArg);
}

int main(int argc, char * argv[]) {

    ARGS args;

    parseArgs(argc, argv, &args);

    if( args.h == SET ) {

        prinHelpAndExit(stdout, EXIT_SUCCESS);
    }

    validateArgs(&args);

    printOptions(&args); //tu by bola funkcionalita programu

    return EXIT_SUCCESS;
}
