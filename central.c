#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


// Structure for an alarm
typedef struct {
    int hour;
    int minute;
    char ringtone[255];
} Alarm;

// TODO: Implement function to set an alarm
void setAlarm(Alarm *alarm) {
    // ...
}

// TODO: Implement function to display current time

// TODO: Implement function to display the timer
void displayTimer() {
    // ...
}

// TODO: Implement function for the stopwatch feature


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

// TODO: Implement function to choose a ringtone
void chooseRingtone(char *ringtone) {
    // ...
}

// TODO: Implement function to handle alarm ringing
void ringAlarm(Alarm *alarm) {
    // ...
}


// Main function
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
        printf("Enter choice: ");
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
                char ringtone[255];
                chooseRingtone(ringtone);
                strcpy(alarm.ringtone, ringtone);
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
