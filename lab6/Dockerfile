#definicja uzycia BuildKit jako frontendu
# syntax=docker/dockerfile:1

#etap 1

#budowanie obrazu na podstawie Alpine 3.21
FROM alpine:3.21 AS stage1

#instalacja niezbędnych narzędzi do pobrania repo z githuba 
RUN apk add --no-cache git openssh-client

#utworzenie katalogu .ssh (jezeli nie istnieje), nadanie mu odpowiednich uprawnien oraz pobranie klucza publicznego z githuba do known_hosts
RUN mkdir -p ~/.ssh \
    && chmod 700 ~/.ssh \
    && ssh-keyscan github.com >> ~/.ssh/known_hosts

#zmienna ARG VERSION - bazowo 1.0.0
ARG VERSION=1.0.0

#deklaracja katalogu roboczego, gdzie zostanie sklonowane repo z githuba 
WORKDIR /usr/app 

#zapisanie zmiennej version do pliku version.txt w /use/app
RUN echo $VERSION > version.txt

#sklonowanie repo z githuba (za pomocą SSH - BuildKit) - w katalogu /usr/app zostanie utworzony katalog pawcho6
RUN --mount=type=ssh git clone git@github.com:IWinter1/pawcho6.git .

#zapisanie zmiennej version do pliku version.txt w /use/app
RUN echo $VERSION > version.txt

#etap 2

#wziecie obrazu nginx w wersji 1.27.0
FROM nginx:1.27.0-alpine AS stage2

#kopiowanie pliku konfiguracyjnego z pobranego repo
COPY --from=stage1 /usr/app/src/default.conf /etc/nginx/conf.d/default.conf

#ustawienie katalogu roboczego na katalog roboczy serwera
WORKDIR /usr/share/nginx/html

#skopiowanie potrzebnych plików konfiguracyjnych z pobranego repo
COPY --from=stage1 /usr/app/src/index.html .
COPY --from=stage1 /usr/app/src/js/ ./js/
COPY --from=stage1 /usr/app/version.txt .

#instalacja curl do HEALTHCHECK
RUN apk add --no-cache curl

#ustawienie healtcheck - sprawdza co 5s, kazdy check ma maks 1s, po sekundzie zaczyna sprawdzac aby zaladowala sie strona za pomoca curl robi sprawdzanie strony 
HEALTHCHECK --interval=5s --timeout=1s --start-period=1s CMD curl -f http://localhost/ || exit 1

#port w konterze na ktorym jest serwer
EXPOSE 80

#domyslne cmd
CMD ["nginx", "-g", "daemon off;"] 
