with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "sdl-sample";
  src = ./src;
  buildInputs = [ gcc SDL2 SDL2.dev ];
  buildPhase = "gcc -o renderer *.c -lSDL2 -std=c99";

  installPhase = ''
    mkdir -p $out/bin
    cp renderer $out/bin/
  '';
}

