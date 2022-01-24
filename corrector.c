//
//  main.c
//  corrector
//
//  Created by Bengisu Özdemir on 09.01.2022.
//
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
char sex[5];
char name[20];
char surname[20];


const char *control(const char *fileName)
{
    const char *dot = strrchr(fileName, '.');
    if(!dot|| dot== fileName)
    {
        return "";
    }
    return dot + 1;
}



void traverse(const char * dirName){
    //traverses parameter directory, closes directory.
    DIR * dir = opendir(dirName);
    if(dir == NULL)
    {
        return;
    }
    struct dirent* features;
    features= readdir(dir);
    while(features != NULL)
    {
        if(features->d_type == DT_DIR && strcmp(features->d_name, ".") != 0 && strcmp(features->d_name, "..")!= 0)
        {
            char path[100] = {0};
            strcat(path, dirName);
            strcat(path, "/");
            strcat(path, features->d_name);
            traverse(path);
        }
        else if(features->d_type == DT_REG && strcmp(control(features->d_name), "txt") == 0 && !(strcmp(features->d_name, "database.txt") == 0 && strcmp(dirName, ".") == 0))
        {
            FILE * feature_file;
            char p[50] = {0};
            strcat(p, dirName);
            strcat(p, "/");
            strcat(p, features->d_name);
            feature_file = fopen(p, "r+");
            char line[256];
            bool check = false;
            while(fscanf(feature_file, "%s", line) != EOF)
            {
                if(strcmp(line, name)==0)
                {
                    check = true;
                }
                if(check)
                {
                    long int backwards = -strlen(line) -4;
                    long int afterwards = strlen(line) + 2;
                    if(sex[0] == 'f')
                    {
                        fseek(feature_file, backwards, SEEK_CUR);
                        fputs("Ms.", feature_file);
                        fseek(feature_file, afterwards, SEEK_CUR);
                        fputs(surname, feature_file);
                    }
                    else if(sex[0] == 'm')
                    {
                        fseek(feature_file, backwards, SEEK_CUR);
                        fputs("Mr.", feature_file);
                        fseek(feature_file, afterwards, SEEK_CUR);
                        fputs(surname, feature_file);
                    }
                }
                check = false;
            }
            fclose(feature_file);
        }
        features = readdir(dir);
    }
    closedir(dir);
}

int main(int argc, const char * argv[]) {
    FILE * dataFile;
    dataFile = fopen("database.txt","r");
    char line[256];
    int count = 0;
    
    while(fscanf(dataFile, "%s", line) != EOF)
    {
       
        
        if(count == 0)
        {
            strcpy(sex, line);
            count++;
        }
        else if(count == 1)
        {
            strcpy(name, line);
            count++;
        }
        else if(count == 2)
        {
            strcpy(surname, line);
            traverse(".");
            count =0;
        }
    }
    fclose(dataFile);
    return 0;
}
