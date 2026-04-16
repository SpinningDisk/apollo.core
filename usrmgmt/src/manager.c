#include <malloc.h>
#include <sqlite3.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define DBLOCATION "/mnt/data/dev/control/core/usrmgmt/DB/"

typedef struct s_AppData{
    void* version;
    char name[];
}t_AppData;

int string_to_int(char* string){
    int val = 0;
    for(int i=strlen(string)-1; i>-1; i--){
        char c = string[i];
        // printf("loop %d\n\tc: %c\n", i, c);
        if(i==0){
            // printf("\tindex 0\n");
            if(c=='-'){
                // printf("\t\tnegative sign\n");
                return val*(-1);
            }
        }
        // printf("\tmain\n");
        // printf("\t\tval += pow(%d, %d)*%d = %d*%d = %d -> val=", 10, i, (int)(c-48), (int)pow((double)10, (double)(strlen(string)-i-1)), (int)(c-48), (int)pow((double)10, (double)(strlen(string)-i-1))*(int)(c-48));
        val += (int)pow((double)10, (double)(strlen(string)-i-1))*(int)(c-'0');
        // printf("%d\n", val);
    }
    return val;
}


int check_table_exists(void* ret, int argc, char **argv, char **column){
    const char* color = "\x1b[34m";
    const char* colorImportant = "\x1b[34;1m";
    printf("\n");
    printf("%s[CHECKSTATUS]\tentring check_table_exists\n\x1b[0m", color);
    if(argc>0){
        if(argv[0][0]=='1'&argv[0][1]==(char)0){
                printf("%s[CHECKSTATUS]\t\tgot one table\n\x1b[0m", colorImportant);
            printf("\n");
            return 0;
        }
        printf("%s[CHECKSTATUS]\t\tfound no table\n\x1b[0m", colorImportant);
            printf("\n");
        return 1;
    }else{
        printf("%s[CHEKSTATUS]\t\tsomething went wrong...\n\x1b[0m", colorImportant);
            printf("\n");
        return -1;
    }
    printf("%s[CHECKSTATUS]\t\tjust... just how???\n\x1b[0m", colorImportant);
    printf("\n");
    return -2;
}
int checkup_app(){
    printf("\n");
    printf("\x1b[36m[CHECKUP]\tEntering app checkup:\n\x1b[0m");
    sqlite3 *db = NULL;
    int ret = sqlite3_open(DBLOCATION"Apps.sql", &db);
    if(ret!=0){
        fprintf(stderr, "\x1b[36;1m[CHECKUP]\t\t[FE]could not open Apps.sql:\n%s\x1b[0m", sqlite3_errmsg(db));
        return 1;
    }else{
        printf("\x1b[36m[CHECKUP]\t\tApps database found\n\x1b[0m");
    }
    char** tableExistsError;
    void* tableExistsRet = (void*)malloc(0);
    int tableExists = sqlite3_exec(db, "SELECT COUNT(TYPE) FROM sqlite_master WHERE TYPE='table' AND name='APPS';", check_table_exists, tableExistsRet, tableExistsError);
    if(tableExists==4){
        fprintf(stderr, "\x1b[36;1m[CHECKUP]\t\tApps database does not appear to contain Apps table\n\x1b[0m");
        sqlite3_exec(db, "CREATE TABLE main.APPS (ID INTEGER PRIMARY KEY, name TEXT NOT NULL, description TEXT, version TEXT NOT NULL)", NULL, NULL, tableExistsError);
        printf("\x1b[36m[CHECKUP]\t\tApps table has been created\n\x1b[0m");
        printf("\x1b[36m[CHECKUP]\t\trestarting...\n\x1b[0m");
        free(tableExistsRet);
        free(tableExistsError);
        return checkup_app();
    }
    

    free(tableExistsRet);
    free(tableExistsError);
    sqlite3_close(db);
    return 0;
}
int register_app(char* name, char* description, void* version, int versionType){
    switch(versionType){
        case 0:
            break;
        case 1:
            break;
        default:
            fprintf(stderr, "Could not register app: Could not infere type of version (between int and string)\n");
            return 1;
    }
 
    sqlite3 *db;
    int ret;
    ret = sqlite3_open("apps.sql", &db);

    if(ret){
        fprintf(stderr, "Coundn't open database \"apps.sql\": %s\n", sqlite3_errmsg(db));
        return 1;
    }else{
        fprintf(stderr, "Opened database successfully\n");
        return 2;
    }
    sqlite3_close(db);
    return 0;
}

int main(){
    checkup_app();
    return 0;
}