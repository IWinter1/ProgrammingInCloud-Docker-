# Dockerfile automatycznie wygenerowany przez docker init zostal nadpisany przez ten plik

# dodanie zmiennej ARG z wartoscia bazowa latest (mozliwosc kontroli wersji ubuntu przy budowaniu obrazu)
ARG UBUNTU_VERSION=latest
FROM ubuntu:${UBUNTU_VERSION}

#dodanie zmiennej do kontenera poprzez ENV - uzytkownik, bedzie mogl sprawdzic w kontenerze, jaka wersje zainstalowal (wartosc domyslna na 24.04 - obecnie latest)
ENV UBUNTU_VERSION=${UBUNTU_VERSION:-24.04}

# maintainer wedlug dokumentacji jest przestarzaly, dlatego zostalo uzyte LABEL
LABEL author="Kowalczyk <s101600@pollub.edu.pl>"

# aktualizacja ubuntu, wyczyszczenie niepotrzebnych plikow (minimalizacja warstw) - wszystko jako 1 komenda, aby zminimalizować rozmiar obrazu
RUN apt-get update && apt-get upgrade -y \
    && apt-get install -y --no-install-recommends apache2 \ 
    && apt-get clean \ 
    && rm -rf /var/lib/apt/lists/*

# ustawienie katalogu roboczego (standardowy dla apache)
WORKDIR /var/www/html

# skopiowanie strony internetowej do katalogu roboczego "." to skrot do sciezki WORKDIR
COPY src/index.html .

# informacja o porcie nasluchiwania Apache
EXPOSE 80/tcp

# uruchomienie serwera apache w trybie Foreground (kontener bedzie dzialac w tle)
ENTRYPOINT ["apachectl"]
CMD ["-D", "FOREGROUND"]
