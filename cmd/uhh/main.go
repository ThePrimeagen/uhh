package main

import (
    "os"
    "github.com/theprimeagen/uhh/internal/add"
    "github.com/theprimeagen/uhh/internal/find"
    "github.com/theprimeagen/uhh/internal/delete"
    "github.com/theprimeagen/uhh/internal/config"
    "github.com/theprimeagen/uhh/internal/types"
)

var functions map[string]types.CmdFunc = map[string]types.CmdFunc{
    "add": add.Add,
    "delete": delete.Delete,
}

func main() {

    c := config.InitConfig();
    args := os.Args[1:]
    cmd := args[0]

    function, ok := functions[cmd]

    if !ok {
        find.FindAndPrint(&c, args)
    } else {
        function(&c, args[1:])
    }
}

