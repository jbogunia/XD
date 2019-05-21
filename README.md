
# Information board

Nasz projekt działa na zasadzie backendu & frontendu. Strona wrzucona jest do pamięci urządzenia dzięki SPIFFS a requesty obsłużone są dzięki [ESP async webserver](https://github.com/me-no-dev/ESPAsyncWebServer).

Z tego co pamiętam na następne labki mamy mieć gotowy prototyp oraz refactoring branchów
Taski które jesteśmy w stanie zrobić (większość jest już ogarnąłem i trzeba będzie się podzielić kodem xD):
  - [Możliwość przeglądania listy ogłoszeń](https://trello.com/c/5gZT7gMu/5-mo%C5%BCliwo%C5%9B%C4%87-przegl%C4%85dania-listy-og%C5%82osze%C5%84)
  - [Możliwość dodawania nowego ogłoszenia (unikanie spamu)](https://trello.com/c/qBlSEFMH/11-mo%C5%BCliwo%C5%9B%C4%87-dodawania-nowego-og%C5%82oszenia-unikanie-spamu)
  - [Usuwanie ogłoszenia na żądanie](https://trello.com/c/a26lpgZA/9-usuwanie-og%C5%82oszenia-na-%C5%BC%C4%85danie)
- [Backup stanu tablicy ogłoszeń](https://trello.com/c/2gUxYJGf/13-backup-stanu-tablicy-og%C5%82osze%C5%84)
- [Możliwość edycji ogłoszenia przez twórcę](https://trello.com/c/H3PujJak/10-mo%C5%BCliwo%C5%9B%C4%87-edycji-og%C5%82oszenia-przez-tw%C3%B3rc%C4%99)

# Backend

Backend jest już praktycznie skończony - został do dokończenia endpoint służący do modyfikowania ogłoszenia oraz poprawa logiki (np validacji odbieranych danych) w pozostałych enpointach. 
Zapis ogłoszeń z pliku jest już ogarnięty i chyba działa ok :) 

# Frontend

Frontend ogarnia Karol & Irmina i wy chyba wiecie co tutaj macie ogarnąć XD
[Dokumentacja API](https://documenter.getpostman.com/view/2823010/S1TN71rS)

# Testy

No i tu jest problem bo trzeba jakieś testy napisać, ale no jak na razie nie mam pomysłu jakie :/ 
Jak na razie API testowałem ręcznie z wykorzystaniem Postmana, ale można by napisac jakieś testy do tego (nie w C!!)

# Podział kodu

Musimy się podzielić kodem (tym co napisałem :D) i każda seekcja musi wrzucić to na swój branch, a potem musimy zrobić refactoring wrzyconych ficzerów. 
Proponuje trzymać się takiej konwencji:
```
if(12 == 12){  //nawiasy w tej samej linii co instrukcja warunkowa
    return true;
}
```
```
class FileAdapter { //nazwa klass camel case z dużej litery
    public:
        bool getAdverts(); //nazwa metod z małej litery camel casem
}
```

