# syntax=docker/dockerfile:1.4
# Stage 1: Build
FROM alpine:3.19 AS builder


# Install necessary tools: compiler, git, and SSH client
RUN apk add --no-cache build-base git openssh-client
WORKDIR /app

# Configure GitHub as a trusted host
RUN mkdir -p -m 0700 ~/.ssh && ssh-keyscan github.com >> ~/.ssh/known_hosts

# Clone ONLY the 'zadanie_1' directory using sparse-checkout and shallow clone
# This saves bandwidth and build time
RUN --mount=type=ssh \
    git clone --depth 1 --filter=blob:none --sparse git@github.com:IWinter1/ProgrammingInCloud-Docker-.git . && \
    git sparse-checkout set zadanie_1/czesc_1

# Change working directory to the downloaded folder
WORKDIR /app/zadanie_1/czesc_1

# Static compilation - optimized for size (-Os) and stripped (-s)
RUN g++ -static -Os -s \
    -ffunction-sections -fdata-sections \
    main.cpp WeatherApp.cpp -o weather_app \
    -Wl,--gc-sections -lpthread -std=c++17 && \
    g++ -static -Os -s \
    -ffunction-sections -fdata-sections \
    healthcheck.cpp -o healthcheck \
    -Wl,--gc-sections -lpthread -std=c++17



# Stage 2: Final Image
FROM scratch

# Metadata and OCI compliance
ARG AUTHOR="Kristian Kowalczyk"
ARG LICENSE="OPEN"
ARG BUILD_DATE
ARG APP_PORT=8080
ENV APP_PORT=$APP_PORT
ENV APP_AUTHOR=$AUTHOR

LABEL org.opencontainers.image.authors=$AUTHOR \
      org.opencontainers.image.licenses=$LICENSE \
      org.opencontainers.image.created=$BUILD_DATE \
      org.opencontainers.image.title="WeatherApp"

# Minimal binaries from builder
COPY --from=builder /app/zadanie_1/czesc_1/weather_app /app/zadanie_1/czesc_1/healthcheck /

USER 1000:1000
# Healthcheck configuration (required in Task 2)
HEALTHCHECK --interval=30s --timeout=3s --start-period=2s --retries=3 \
    CMD ["/healthcheck"]

EXPOSE 8080
ENTRYPOINT ["/weather_app"]