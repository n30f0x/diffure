{
  stdenv,
  pkgs,
  gnumake,
  gccStdenv,
}:

stdenv.mkDerivation {
 pname = "diffure";
 version = "0.0.4";

 src = pkgs.lib.fileset.toSource {
   root = ./.;
   fileset = ./.;
 };

buildInputs = [ gnumake gccStdenv ];

configurePhase = ''
  make dry
'';

installPhase = ''
  make
  mkdir -p $out/bin/
  cp build/diffure $out/bin/diffure
'';

 meta = with pkgs.lib; {
  description = "a tiny patching companion";
  license = licenses.gpl3;
  author = "n30f0x";
  maintainers = [ "n30f0x" ];
 };
}

