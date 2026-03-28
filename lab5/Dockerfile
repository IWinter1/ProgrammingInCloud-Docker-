#etap 1
FROM scratch AS stage1

#w katalogu src znajduja sie wszystkie potrzebne pliki
ADD ./src/alpine-minirootfs-3.23.3-x86_64.tar.gz /

#zmienna ARG VERSION - bazowo 1.0.0
ARG VERSION=1.0.0

#deklaracja katalogu roboczego dla aplikacji 
WORKDIR /usr/app 

#skopiowanie bazowej wersji html oraz skryptu js do sciezki z workdir 
COPY ./src/index.html .
COPY ./src/js/script.js ./js/

#zapisanie zmiennej version do pliku version.txt w /use/app
RUN echo $VERSION > version.txt

#etap 2

#wziecie obrazu nginx w wersji 1.27.0
FROM nginx:1.27.0-alpine AS stage2

#przeniesienie pliku konfiguracyjnego dla nginx do odpowiedniego katalogu
COPY ./src/default.conf /etc/nginx/conf.d/default.conf

#ustawienie katalogu roboczego na katalog roboczy serwera
WORKDIR /usr/share/nginx/html

COPY --from=stage1 /usr/app/ .

#instalacja curl do HEALTHCHECK
RUN apk add --no-cache curl

#ustawienie healtcheck - sprawdza co 5s, kazdy check ma maks 1s, po sekundzie zaczyna sprawdzac aby zaladowala sie strona za pomoca curl robi sprawdzanie strony 
HEALTHCHECK --interval=5s --timeout=1s --start-period=1s CMD curl -f http://localhost/ || exit 1

#port w konterze na ktorym jest serwer
EXPOSE 80

#domyslne cmd
CMD ["nginx", "-g", "daemon off;"] 
