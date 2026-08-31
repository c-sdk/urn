{
  description = "URN parser library for C";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forAllSystems = nixpkgs.lib.genAttrs systems;
    in
    {
      packages = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          default = pkgs.stdenv.mkDerivation {
            pname = "urn";
            version = "0.1.0";
            src = self;

            nativeBuildInputs = [
              pkgs.clang
              pkgs.pkg-config
            ];

            makeFlags = [
              "CC=clang"
              "PREFIX=$(out)"
            ];

            doCheck = true;
            checkTarget = "test";
          };
        });

      devShells = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          default = pkgs.mkShell {
            packages = [
              pkgs.clang
              pkgs.gnumake
              pkgs.pkg-config
              pkgs.clib
            ];
          };
        });
    };
}
