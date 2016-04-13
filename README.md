# [SOPS1] Projekt 1: Demon synchronizujący dwa podkatalogi
Program który otrzymuje co najmniej dwa argumenty: ścieżkę źródłową, ścieżkę docelowa . Jeżeli któraś ze ścieżek nie jest katalogiem program powraca natychmiast z komunikatem błędu. W przeciwnym wypadku staje się demonem. Demon wykonuje następujące czynności: śpi przez piec minut (czas spania można zmieniać przy pomocy dodatkowego opcjonalnego argumentu), po czym po obudzeniu się porównuje katalog źródłowy z katalogiem docelowym. Pozycje, które nie są zwykłymi plikami są ignorowane (np. katalogi i dowiązania symboliczne). Jeżeli demon **(a)** napotka na nowy plik w katalogu źródłowym, i tego pliku brak w katalogu docelowym lub **(b)** plik w katalogu docelowym ma późniejsza datę ostatniej modyfikacji demon wykonuje kopie pliku z katalogu źródłowego do katalogu docelowego - ustawiając w katalogu docelowym datę modyfikacji tak, aby przy kolejnym obudzeniu nie trzeba było wykonać kopii (chyba ze plik w katalogu źródłowym zostanie ponownie zmieniony). Jeżeli zaś odnajdzie plik w katalogu docelowym, którego nie ma w katalogu źródłowym to usuwa ten plik z katalogu docelowego. Możliwe jest również natychmiastowe obudzenie się demona poprzez wysłanie mu sygnału *SIGUSR1*. Wyczerpująca informacja o każdej akcji typu uśpienie/obudzenie się demona (naturalne lub w wyniku sygnału), wykonanie kopii lub usuniecie pliku jest przesłana do logu systemowego. Informacja ta powinna zawierać aktualna datę. ***(14p)***.

Dodatkowo:
- Opcja *-R* pozwalająca na rekurencyjną synchronizację katalogów (teraz pozycje będące katalogami nie są ignorowane). W szczególności jeżeli demon stwierdzi w katalogu docelowym podkatalog którego brak w katalogu źródłowym powinien usunąć go wraz z zawartością. ***(10p)***
- W zależności od rozmiaru plików dla małych plików wykonywane jest kopiowanie przy pomocy read/write a w przypadku dużych przy pomocy mmap/write (plik źródłowy) zostaje zamapowany w całości w pamięci. Próg dzielący pliki małe od dużych może być przekazywany jako opcjonalny argument. ***(10p)***



### Zarys działania programu
###### *1. Parametry wejściowe:*

        ścieżka źródłowa (*) - musi być katalogiem
        ścieżka docelowa (*) - musi być katalogiem
        -R (opcjonalny) - oznacza rekurencyjna synchronizacja katalogów
        -T czas_snu (opcjonalny) - oznacza ustawienie innagegaeagegaeego czasu snu demona niż domyślne 5 minut
        -S prog - oznacza ustawienie progu dzielącego pliki małe od dużych
        -V - oznacza tryb "verbose" (logowanie do sysloga i konsoli)



###### *2. Przebieg:*
1. uruchomienie programu

2. weryfikacja argumentów
    - brak któregoś z argumentów, lub sytuacja, w której argument to nie katalog -> zakończenie działania z komunikatem błędu

3. porównanie katalogu głównego z katalogiem docelowym pod kątem różnic
    - brak parametru *-R* skutkuje nie sprawdzaniem podkatalogów
    - napotkanie na nowy plik w katalogu źródłowym (brak pliku w docelowym) -> kopia
    - plik w katalogu docelowym ma wcześniejszą datę ostatniej modyfikacji -> kopia i ustawienie datę modyfikacji na odpowiednią
    - brak pliku w katalogu źródłowym (który jest w docelowym) -> usunięcie pliku z katalogu docelowego

4. wykonanie kopii lub usunięcie pliku powoduje przesłanie logu do logów systemowych (z datą i godziną)

5. demon śpi przez 5 minut

6. w przypadku otrzymania sygnału *SIGUSR1* demon jest wybudzany na siłę



###### *3. Dodatek:*
 * inna forma kopiowania plików (zamiast read/write): mmap/write (mapowanie w całości w pamięci)
 * próg dzielący pliki małe od dużych: dodatkowy opcjonalny parametr


### Lista TO-DO jest w zakadce "Issues"!
 
