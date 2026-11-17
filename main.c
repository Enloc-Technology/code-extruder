#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SOURCE_LENGTH 512
#define MAX_ID_LENGTH 31
#define CRITICAL_THRESHOLD 0.75f

typedef enum {
    TARGET_C,
    TARGET_JAVA
} Target;

typedef struct {
    char patient[MAX_ID_LENGTH + 1];
    float severity;
    bool sulking;
    Target target;
} Input;

static bool valid_id(const char *text) {
    size_t length;

    if (text == NULL) {
        return false;
    }
    length = strlen(text);
    if (length == 0 || length > MAX_ID_LENGTH) {
        return false;
    }
    for (size_t i = 0; i < length; ++i) {
        unsigned char ch = (unsigned char)text[i];
        if (!isalnum(ch) && ch != '_' && ch != '-' && ch != '.') {
            return false;
        }
    }
    return true;
}

static bool parse_severity(const char *text, float *severity) {
    char *end = NULL;
    float value;

    if (text == NULL || severity == NULL || text[0] == '\0') {
        return false;
    }
    errno = 0;
    value = strtof(text, &end);
    if (errno == ERANGE || end == text || *end != '\0' || !isfinite(value) ||
        value < 0.0f || value > 1.0f) {
        return false;
    }
    *severity = value;
    return true;
}

static bool parse_source(const char *source, Input *input) {
    char buffer[MAX_SOURCE_LENGTH + 1];
    bool seen_patient = false;
    bool seen_severity = false;
    bool seen_sulking = false;
    bool seen_target = false;
    size_t source_length;

    if (source == NULL || input == NULL) {
        return false;
    }
    source_length = strlen(source);
    if (source_length == 0 || source_length > MAX_SOURCE_LENGTH) {
        return false;
    }

    *input = (Input){
        .patient = "Tech-Agent-404",
        .severity = 0.2f,
        .sulking = false,
        .target = TARGET_C
    };
    memcpy(buffer, source, source_length + 1);

    for (char *token = strtok(buffer, " \t\r\n"); token != NULL;
         token = strtok(NULL, " \t\r\n")) {
        char *equals = strchr(token, '=');
        const char *value;

        if (equals == NULL || equals == token || equals[1] == '\0' ||
            strchr(equals + 1, '=') != NULL) {
            return false;
        }
        *equals = '\0';
        value = equals + 1;

        if (strcmp(token, "patient") == 0) {
            if (seen_patient || !valid_id(value)) {
                return false;
            }
            memcpy(input->patient, value, strlen(value) + 1);
            seen_patient = true;
        } else if (strcmp(token, "severity") == 0) {
            if (seen_severity || !parse_severity(value, &input->severity)) {
                return false;
            }
            seen_severity = true;
        } else if (strcmp(token, "sulking") == 0) {
            if (seen_sulking ||
                (strcmp(value, "true") != 0 && strcmp(value, "false") != 0)) {
                return false;
            }
            input->sulking = strcmp(value, "true") == 0;
            seen_sulking = true;
        } else if (strcmp(token, "target") == 0) {
            if (seen_target) {
                return false;
            }
            if (strcmp(value, "c") == 0) {
                input->target = TARGET_C;
            } else if (strcmp(value, "java") == 0) {
                input->target = TARGET_JAVA;
            } else {
                return false;
            }
            seen_target = true;
        } else {
            return false;
        }
    }
    return true;
}

static void print_usage(const char *program) {
    fprintf(stderr,
        "Usage: %s [patient-id [severity]]\n"
        "       %s --source 'patient=ID severity=0.0 sulking=false target=c|java'\n",
        program, program);
}

int main(int argc, char **argv) {
    Input input = {
        .patient = "Tech-Agent-404",
        .severity = 0.2f,
        .sulking = false,
        .target = TARGET_C
    };

    if (argc == 3 && strcmp(argv[1], "--source") == 0) {
        if (!parse_source(argv[2], &input)) {
            fprintf(stderr, "Invalid source input; expected bounded key=value fields.\n");
            return EXIT_FAILURE;
        }
    } else {
        if (argc > 3) {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        if (argc >= 2) {
            if (!valid_id(argv[1])) {
                fprintf(stderr, "Patient ID must be 1-%d letters, digits, '.', '_' or '-'.\n",
                        MAX_ID_LENGTH);
                return EXIT_FAILURE;
            }
            memcpy(input.patient, argv[1], strlen(argv[1]) + 1);
        }
        if (argc == 3 && !parse_severity(argv[2], &input.severity)) {
            fprintf(stderr, "Severity must be a finite number from 0.0 to 1.0.\n");
            return EXIT_FAILURE;
        }
    }

    printf("Patient: %s\nSeverity: %.2f\nTarget: %s\n",
           input.patient, input.severity,
           input.target == TARGET_C ? "C" : "Java");

    if (input.sulking || input.severity >= CRITICAL_THRESHOLD) {
        puts("Diagnostic: critical Hauvies state; generation stopped safely.");
        return EXIT_FAILURE;
    }

    puts("Diagnostic: input accepted.");
    puts(input.target == TARGET_C
        ? "Route: C starter template selected (not generated or compiled)."
        : "Route: Java starter template selected (not generated or compiled).");
    return EXIT_SUCCESS;
}
