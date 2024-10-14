# Usa la imagen base de Rust
FROM rust:latest as builder

# Establece el directorio de trabajo
WORKDIR /usr/src/server-ingenieria

# Copia los archivos necesarios para compilar
COPY Cargo.toml Cargo.lock ./
RUN mkdir src
COPY src/main.rs src/main.rs

# Agrega el objetivo musl
RUN rustup target add x86_64-unknown-linux-musl

# Instala musl-tools
RUN apt-get update && apt-get install -y musl-tools

# Compila la aplicación en modo release
RUN cargo build --release --target=x86_64-unknown-linux-musl

# Usa una imagen base ligera para el runtime
FROM scratch
COPY --from=builder /usr/src/server-ingenieria/target/x86_64-unknown-linux-musl/release/server-ingenieria /usr/local/bin/

# Comando por defecto al iniciar el contenedor
CMD ["server-ingenieria"]
