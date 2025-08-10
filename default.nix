{
  pkgs,
  lib,
  stdenv,
  # gccStdenv,
  autoreconfHook,
  gnumake,
  ccls,
  buildType ? "default",
  withHardening ? true,
  checkBuild ? true,
}:

stdenv.mkDerivation rec {
  pname = "diffure";
  version = builtins.elemAt ( builtins.match ".*([0-9]+\\.[0-9]+\\.[0-9]+).*"
    ( builtins.readFile ./configure.ac ) ) 0; #? "0.0.0";

  src = pkgs.lib.fileset.toSource {
    root = ./.;
    fileset = ./.;
  };
 
  buildInputs = [ gnumake autoreconfHook ];
  propagatedBuildInputs = [ ccls ];
  
  enableParallelBuild = true;
  
  makeFlags = [
    "BUILD=./"
    "EXENAME=${pname}-${version}"
  ];
  
  preBuildPhase = lib.optional (checkBuild || (buildType != "default") ) ''make dry'';

  buildPhase = (''
    runHook preBuildPhase
  '' + {
    test = "make test";
    debug = "make debug";
    default = "make" + lib.optionalString (!withHardening) " pure";
  }."${buildType}");
  
  installPhase = ''
    mkdir -p $out/bin/
    cp build/diffure $out/bin/diffure
  '';
  
  doCheck = true;
 
  meta = with lib; {
   description = "a tiny patching companion";
   license = licenses.gpl3;
   author = "n30f0x";
   maintainers = [ "n30f0x" ];
  };
}

