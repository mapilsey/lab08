#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

int main() {
    // Установка обработчика сигнала SIGINT (Ctrl+C)
    signal(SIGINT, handle_sigint);
    
    // Получение пути к файлу логов из переменной окружения
    const char* log_path = getenv("LOG_PATH");
    if (!log_path) {
        fprintf(stderr, "LOG_PATH environment variable not set!\n");
        return 1;
    }
    
    // Открытие файла логов для записи (создаем при необходимости)
    FILE* log_file = fopen(log_path, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return 1;
    }
    
    printf("Enter text (press Ctrl+C to exit):\n");
    
    char buffer[BUFFER_SIZE];
    while (keep_running) {
        // Чтение ввода пользователя
        if (fgets(buffer, BUFFER_SIZE, stdin)) {
            // Удаление символа новой строки
            buffer[strcspn(buffer, "\n")] = '\0';
            
            // Запись в файл логов
            fprintf(log_file, "[PID:%d] %s\n", getpid(), buffer);
            fflush(log_file);  // Немедленная запись в файл
            
            printf("Logged: %s\n", buffer);
        } else if (feof(stdin)) {
            // Обработка конца файла (Ctrl+D)
            printf("\nEOF detected. Exiting.\n");
            break;
        } else {
            perror("Error reading input");
            break;
        }
    }
    
    // Запись сообщения о завершении
    fprintf(log_file, "Application stopped by %s\n", 
            keep_running ? "EOF" : "SIGINT");
    fclose(log_file);
    
    printf("Log saved to: %s\n", log_path);
    return 0;
}
