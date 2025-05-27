{
  description = "Tiny patching companion";

  nixConfig = {
    extra-experimental-features = [ "nix-command" "flakes" ];
  };

  inputs = {
    nixpkgs = {
      url = "flake:nixpkgs/nixpkgs-unstable";
    };
  };

  outputs = { self, nixpkgs }:
   let          
      forSystem = f: nixpkgs.lib.genAttrs nixpkgs.lib.systems.flakeExposed
       (system: f {
          pkgs = import nixpkgs { inherit system; };
      });
   in {
        packages = forSystem (
        { pkgs }: rec
         {
          diffure = pkgs.callPackage ./diffure.nix { inherit pkgs; };
          default = diffure;
         } 
        );
     };
}
