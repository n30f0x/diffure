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

  outputs = { nixpkgs, ... } :
   let
    
      linuxSystems  = [ "x86_64-linux" "aarch64-linux" ];
      darwinSystems = [ "x86_64-darwin" "aarch64-darwin" ];
      allSystems = linuxSystems ++ darwinSystems;     
      
      forSystem = systemsIN: f: nixpkgs.lib.genAttrs systemsIN
       (system: f {
          pkgs = import nixpkgs { inherit overlays system; };
      }); 

      overlays = [
        (self: super: {
        })
      ];

   in {

      packages = forSystem allSystems (
      { pkgs }: rec
       {
        diffure = pkgs.callPackage ./default.nix { inherit pkgs; };
        default = diffure;
       } 
      );

    };
}
