package main

import (
    "uhh/config"
    "os"
    "encoding/json"
)

func main() {
    c := config.InitConfig();

    json.NewEncoder(os.Stdout).Encode(&c)
}

