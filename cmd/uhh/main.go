package main

import (
    "uhh/internal/config"
    "uhh/internal/types"
    "os"
)

func contains(arr []string, str string) bool {
   for _, a := range arr {
      if a == str {
         return true
      }
   }
   return false
}

var functions map[string]types.CmdFunc

func main() {
    functions = map[string]types.CmdFunc{}

    c := config.InitConfig();
    args := os.Args
    cmd := args[0]
    args = args[1:]

    function, ok := functions[cmd]
    if !ok {
        // search
    } else {
        function(&c, args)
    }
}

