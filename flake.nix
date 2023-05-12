{
  description = "Executor 2000 - classic mac emulator";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      perSystem = { config, self', inputs', pkgs, system, ... }: {
        packages.default = pkgs.stdenv.mkDerivation
          {
            name = "executor2000";
            nativeBuildInputs = with pkgs; [ cmake bison perl ruby pkgconfig qt5.wrapQtAppsHook ];
            buildInputs = with pkgs; [
              qt5.qtbase
              boost
              readline
              SDL2
            ] ++ lib.optionals stdenv.isLinux [
              SDL
              wayland
              waylandpp
            ] ++ lib.optionals stdenv.isDarwin (with darwin.apple_sdk.frameworks; [
              Carbon
              Cocoa
            ]);
            src = ./.;
            hardeningDisable = [ "all" ];
            cmakeFlags = [ "-DRUN_FIXUP_BUNDLE=NO" "-DNO_STATIC_BOOST=YES" ];
          };
      };
    };
}
