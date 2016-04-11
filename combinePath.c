/*
Tworzenie jednej polaczonej nazwy KATALOG + / + NAZWA_PLIKU
*/

void combinePath(char* combined, const char* katalog, const char* nazwa)
{
    if(katalog == NULL && nazwa == NULL) {
        strcpy(combined, "");
    }
    else if(nazwa == NULL || !nazwa[0]) {
        strcpy(combined, katalog);
    }
    else if(katalog == NULL || !katalog[0]) {
        strcpy(combined, nazwa);
    } 
    else {
        char directory_separator[1] = "/";
        strcpy(combined, katalog);
		strcat(combined, directory_separator);
        strcat(combined, nazwa);
    }
}