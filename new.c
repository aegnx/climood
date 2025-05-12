#include <stdio.h>
#include <stdlib.h>  
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

int main(void)
{
    char *HomeEnvVar = getenv("HOME");
    if (HomeEnvVar == NULL)
    {
        fprintf(stderr, "Error: getenv failed!/$HOME is not set!\n");
	return EXIT_FAILURE;
    }
    char *Username = getenv("USER");
    if (Username == NULL)
    {
        perror("Error: getenv failed!/$USER is not set!\n");
        return 1;
    }
    char ConfigDirectory[512];
    snprintf(ConfigDirectory, sizeof(ConfigDirectory), "%s/.config/climood", HomeEnvVar);
    char HistFile[512];
    snprintf(HistFile, sizeof(HistFile), "%s/hist", ConfigDirectory);
    if (mkdir(ConfigDirectory, 0755) != 0)
    {
        #include <errno.h>
        if (errno != EEXIST)
        {
            perror("Can't create directory, weird...\n");
	    return EXIT_FAILURE;
        }
    }
    FILE *fp = fopen(HistFile, "a");
    if(fp == NULL)
    {
        perror("fopen failed!\n");
        return EXIT_FAILURE;
    }
    time_t now = time(NULL);
    if (now == ((time_t)-1))
    {
        perror("Can't get time, weird...\n");
	fclose(fp);
	return EXIT_FAILURE;
    }
    int CurrentMood;
    printf("hello %s! what would you rate your mood on a scale of 1-5?\n", Username);
    printf("1. Very happy!\n");
    printf("2. Happy!\n");
    printf("3. Neutral\n");
    printf("4. A bit down...\n");
    printf("5. Ughh literally so sad\n");
    printf("Sooooo what would you rate it?:");
    if (scanf("%d", &CurrentMood) != 1)
    {
        printf("Can't get input, weird...\n");
	return 1;
    }
    switch (CurrentMood)
    {
        case 1:
	    printf("Set mood: Very happy\nWriting to file...\n");
	    break;
	case 2:
	    printf("Set mood: Happy\nWriting to file...\n");
	    break;
	case 3:
	    printf("Set mood: Neutral\nWriting to file...\n");
	    break;
	case 4:
	    printf("Set mood: Sad\nWriting to file...\n");
	    break;
	case 5:
	    printf("Set mood: Very sad\nWriting to file...\n");
	    break;
	default:
	    printf("Huh what...\n");
	    return EXIT_FAILURE;
    }
    int NoteChoice;
    printf("Wanna add a note?(1. yes, 2. no):");
    if (scanf("%d", &NoteChoice) != 1)
    {
        printf("Can't get input, weird...\n");
	return EXIT_FAILURE;
    }
    char Note[512];
    switch (NoteChoice)
    {
        case 1:
            printf("Heck yeah! Enter your note here:");
	    if (scanf(" %[^\n]", Note) != 1)
	    {
                perror("Can't get input, weird...\n");
	    }
	    break;
	case 2:
	    break;
    }
    if (fprintf(fp, "%ld %d %s\n", now, CurrentMood, Note) < 0)
    {
        perror("Can't write to file, weird...\nThis mood wasnt saved TwT\n");
	fclose(fp);
	return EXIT_FAILURE;
    }
    fclose(fp);
    printf("Mood saved!\n");
    return 0;
}
