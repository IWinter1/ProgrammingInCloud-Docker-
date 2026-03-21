### Opis
Ten projekt tworzy obraz Dockera o nazwie web100 oparty na najnowszej wersji Ubuntu (24.04 – stan na 21.03.2026):
    - Dockerfile umożliwia modyfikację wersji Ubuntu podczas budowania poprzez argument UBUNTU_VERSION.
    - Wartość tego argumentu jest również przekazywana do kontenera jako zmienna środowiskowa $UBUNTU_VERSION (domyślnie 24.04), co pozwala na szybkie sprawdzenie wersji systemu wewnątrz kontenera.
    - W kontenerze instalowany jest serwer Apache, który uruchamia prostą stronę internetową zawierającą imię, nazwisko oraz grupę dziekańską na porcie 8888:80 (port zewnętrzny - 8888, port w kontenerze - domyślny dla Apache 80)
Po zbudowaniu obrazu za pomocą komendy "docker history" została sprawdzona ilość wartsw:
    - ilość wszystkich warstw - 14
    - warstwy zajmujące pamięć (!= 0B) - 4:
        - COPY src/index.html . # buildkit
        - WORKDIR /var/www/html
        - RUN /bin/sh -c apt-get update && apt-get upg…
        - /bin/sh -c #(nop) ADD file:3f78aa860931e0853… 

### Budowanie obrazu za pomocą Dockerfile - [] opcjonalne
Do budowania potrzebny będzie w katalogu, w którym znajduje się ten Dockerfile katalog src, w którym musi znaleźć się plik index.html
    `docker build [--build-arg UBUNTU_VERSION=22.04] [-t nazwa] .`
        --build-arg UBUNTU_VERSION=22.04 - pozwala na wybranie wersji ubuntu
        -t nazwa - pozwala na nadanie nazwy (tagu) obrazowi

### Tworzenie i uruchamianie kontenera - [] opcjonalne
Przy budowaniu wykorzystywany jest plik composer.yaml np. z repo z githuba - domyślna konfiguracja - port: 8888:80, container_name: lab4_web
    `docker compose up -d [--build]` 
        --build - opcjonalny, gdy obraz nie został jeszcze zbudowany lub ma być przebudowany (tworzy obraz o nazwie web100)

Bez pliku composer.yaml
    `docker run -d -p 8888:80 [--name nazwa] web100`
        -d - w tle
        -p - port zdefiniowac np. 8888:80
        --name nazwa - opcjonalnie nazwa kontenera
        web100 lub inna nadana nazwa obrazu
        mozna uzyć jeszcze innych flag (np. -ti, aby sprawdzić czy działą zmienna środowiskowa)

### Zatrzymanie kontenera jezeli byl w trybie -d
    `docker stop lab4_web100`
        lub inna nadana nazwa kontenera

### Uruchomienie strony
W przeglądarce wejście na strone 127.0.0.1:8888 lub localhost:8888
    Jeżeli został zdefiniowany inny port zewnętrzny, należy wtedy zamienić 8888 na odpowiedni port

### Obraz na Docker  

[iwinter1/university-lab](https://hub.docker.com/r/iwinter1/university-lab)
