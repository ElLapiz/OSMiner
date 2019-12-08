int * parse_char_to_int(char *char_value, int int_length) {
    int int_value[int_length];
    int i = 0;

    while(char_value != NULL){
        if(isNumeric(char_value)){
            int_value[i] = atoi(char_value);
            printf("%d\n", int_value[i]);
            i = i+1;
        }
        char_value = strtok(NULL, " ");
    }
    return int_value;
}

int isNumeric (const char * s){
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}
