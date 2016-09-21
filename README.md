## to have access on your local computer... 
click green box ... clone
copy that address: https://github.com/MichaelDimmitt/Master_MPI.git
in terminal type git clone --recursive https://github.com/MichaelDimmitt/Master_MPI.git 

## Cool things you can do when you have the program contents in local directory.

https://github.com/miketestgit01/Bash_Directory_CLOC-Count-Lines-of-Program/

# File_Org_Instructions


Steps that built this file organization. 

(1) create local folder name: "Master_MPI"

(2) run command on local folder "git init".

(3) git submodule add https://github.com/JuliaParallel/MPI.jl
(above was an example. The generic command is... "git submodule add PATH")
(you can add any individuals repositories including your own as submodules.)


(3) create empty repository on github named Master_MPI


(4) git add --all 

(5) git commit -m "added submodules"

(6) git add remote https://github.com/Username/Master_MPI/

(7) git push -u origin master


FINISHed
