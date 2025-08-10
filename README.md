       _____   __  ______ ______              
      /\  __-./\ \/\  ___/\  ___\             
      \ \ \/\ \ \ \ \  __\ \  __\             
       \ \____-\ \_\ \_\  \ \_\               
        \/____/ \/_/\/_/   \/_/               
                   __  __  ______  ______     
                  /\ \/\ \/\  == \/\  ___\    
                  \ \ \_\ \ \  __<\ \  __\     
                   \ \_____\ \_\ \_\ \_____\  
                    \/_____/\/_/ /_/\/_____/  

# Diffure - a Tiny cli patching companion

Allows you to make patches on the go with your fav ide or editor.

To run with nix just run `nix shell github:n30f0x/diffure`
Before building run `autoreconf --install && ./configure`
To navigate through make run `make help`


This tool allows you to easily make patches as if you were editing file directly, so you could apply results later in declarative way.

Usage: diffure | options | file ... file

-p  --print:      Print patch into stdout, don't generate file,
                  negated by output specification

-o  --output:     Where to put patchfiles, pwd is default

-s  --skip:       Skip editing resulting patchfile

-r  --reverse:    Reverse order of diffed files

-e  --editor:     Editor of your choice, falls back to env vars if
                  none supplied

-d  --difftool:   Diff tool of your preference, by default uses
                  *nix diff
                  To pass arguments to either option use as 'tool -arg -arg'
