#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>


// Structure for an alarm
typedef struct {
    int hour;
    int minute;
    char ringtone[255];
} Alarm;

char ringtone[255] = "HappyDays.mp3";


void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void displayTime(int hours, int minutes, int seconds) {
    printf("Timer: %02d:%02d:%02d\n", hours, minutes, seconds);
}


int isKeyHit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

void ring() {
    printf("Ringing!\n");
    char command[300];
    sprintf(command, "mpg123 %s", ringtone);
    system(command);
    sleep(1);

}


void setAlarm(Alarm *alarm) {
    printf("Enter alarm time (HH MM): ");
    scanf("%d %d", &alarm->hour, &alarm->minute);

    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);

    int currentHour = localTime->tm_hour;
    int currentMinute = localTime->tm_min;

    int alarmInMinutes = (alarm->hour * 60 + alarm->minute) - (currentHour * 60 + currentMinute);
    if (alarmInMinutes < 0) {
        alarmInMinutes += 24 * 60;  // Adjust for next day
    }

    printf("Alarm will ring in %d hours and %d minutes.\n", alarmInMinutes / 60, alarmInMinutes % 60);

    while (1) {
        time(&currentTime);
        localTime = localtime(&currentTime);
        currentHour = localTime->tm_hour;
        currentMinute = localTime->tm_min;

        if (currentHour == alarm->hour && currentMinute == alarm->minute) {
            ring();
            printf("Press enter to stop ringing.\n");
            while (1){
                if (isKeyHit()) {
                    system("clear");
                    printf("Exiting in 10 seconds.\n");
                    sleep(10);
                    break;
                }
            }
            break;
        }

        sleep(60);
    }
}

void displayTimer() {
    int hours = 0, minutes = 0, seconds = 0;

    time_t start, now, pauseStart, pauseEnd;
    double elapsed, pauseDuration = 0;

    time(&start);  // Record start time

    while (1) {  // Change this to your exit condition
        time(&now);
        elapsed = difftime(now, start) - pauseDuration;

        hours = (int)elapsed / 3600;
        minutes = ((int)elapsed % 3600) / 60;
        seconds = (int)elapsed % 60;

        system("clear");

        displayTime(hours, minutes, seconds);  // Display updated time

        if (isKeyHit()) {
            time(&pauseStart);
            system("clear");
            printf("Timer paused. Press 'c' to continue or any other key to exit.\n");
            printf("\n");
            displayTime(hours, minutes, seconds);

            while (1) {
                char ch = getchar();
                if (ch == 'c' || ch == 'C') {
                    time(&pauseEnd);
                    pauseDuration += difftime(pauseEnd, pauseStart);
                    break;
                } else if (ch != '\n') {
                    clearBuffer();
                    return;
                }
            }
        }

        sleep(1);
    }
}

void stopwatch() {
    int hours, minutes, seconds;

    printf("Enter time (HH MM SS): ");
    scanf("%d %d %d", &hours, &minutes, &seconds);

    int totalSeconds = hours * 3600 + minutes * 60 + seconds;

    time_t start, now, pauseStart, pauseEnd;
    double elapsed, pauseDuration = 0;

    time(&start);

    while (totalSeconds > 0) {
        time(&now);
        elapsed = difftime(now, start) - pauseDuration;

        int remainingSeconds = totalSeconds - (int) elapsed;
        if (remainingSeconds <= 0) {
            system("clear");
            printf("Time's up!\n");
            ring();
            sleep(3);
            return;
        }

        hours = remainingSeconds / 3600;
        minutes = (remainingSeconds % 3600) / 60;
        seconds = remainingSeconds % 60;

        system("clear");
        printf("Press enter to pause\n");
        displayTime(hours, minutes, seconds);

        if (isKeyHit()) {
            time(&pauseStart);
            system("clear");
            printf("Timer paused. Press 'c' and enter to continue or any other key and enter to exit.\n");
            printf("\n");
            displayTime(hours, minutes, seconds);


            while (1) {
                char ch = getchar();
                if (ch == 'c' || ch == 'C') {
                    time(&pauseEnd);  // Record pause time
                    pauseDuration += difftime(pauseEnd, pauseStart);
                    break;
                } else if (ch != '\n') {
                    clearBuffer();
                    return;
                }
            }
        }


        sleep(1);
    }
}

//ringtones gotten from this website: https://www.chosic.com/free-music/ringtones/
//for this to work the mpg123 library must be installed on linux: sudo apt-get install mpg123
void chooseRingtone(char *ringtone) {
    printf("Enter the file name of the ringtone (e.g., ringtone.mp3): ");
    scanf("%s", ringtone);
    ring();

}

int main() {
    Alarm alarm;
    int choice;


    while (1) {
        system("clear");
        time_t currentTime;
        struct tm *localTime;
        char timeBuffer[80];

        time(&currentTime);
        localTime = localtime(&currentTime);
        strftime(timeBuffer, 80, "Current Time: %H:%M\n", localTime);
        printf("%s", timeBuffer);

        printf("1. Set Alarm\n");
        printf("2. Timer\n");
        printf("3. Stopwatch\n");
        printf("4. Choose Ringtone\n");
        printf("5. Exit\n");
        printf("Enter choice as number: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1:
                setAlarm(&alarm);
                break;
            case 2:
                displayTimer();
                break;
            case 3:
                stopwatch();
                break;
            case 4: {
                chooseRingtone(ringtone);
            }
                break;
            case 5:
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}
