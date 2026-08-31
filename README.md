# urn

URN parser library for C.

## Build

```sh
make
make test
```

The default compiler is `clang`; override it with `make CC=...` if needed.

```sh
clib install
make test
```

## Nix

```sh
nix develop
make test
```

Build the package with:

```sh
nix build
```

## pkg-config

Generate and validate the package metadata:

```sh
make pkg-config
```

After installation, consumers can use:

```sh
pkg-config --cflags --libs urn
```

## clib

This project includes `clib.json` for clib-based consumption:

```sh
clib install c-sdk/urn
```
