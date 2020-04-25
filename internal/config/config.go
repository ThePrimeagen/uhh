package config

import (
    "io/ioutil"
    "strings"
    "path"
    "github.com/theprimeagen/uhh/internal/types"
    "golang.org/x/crypto/ssh/terminal"
    "os"
    "os/exec"
    "log"
    "fmt"
)

func readConfig() {
    dat, err := ioutil.ReadFile("/tmp/dat")

    if err != nil {
        log.Fatalf("%+v\n", err)
    }

    fmt.Print(string(dat))
}

func InitConfig() types.Config {
    config, err := os.UserConfigDir()

    if err != nil {
        log.Fatalf("%+v\n", err)
    }

    uhh := path.Join(config, "uhh")
    repo := path.Join(uhh, "repo")
    uhhConfig := path.Join(uhh, ".config")

    _ = os.Mkdir(uhh, os.ModePerm)

    _, err = os.Stat(uhhConfig)
    if os.IsNotExist(err) {

        fmt.Println("Before Uhh can work, you need to provide a repo to save to!");
        fmt.Println("Please enter your repo meow: ");

        t := terminal.NewTerminal(os.Stdin, ">")
        repoUrl, err := t.ReadLine()

        if (err != nil) {

        }

        if err != nil {
            log.Fatalf("%+v\n", err)
        }
        cmd := strings.Split(fmt.Sprintf("git clone %s repo", repoUrl), " ")

        _, err = exec.Command(cmd[0], cmd[1:]...).Output()

        if err != nil {
            log.Fatalf("%+v\n", err)
        }

        ioutil.WriteFile(uhhConfig, []byte("repo=" + repoUrl), os.ModePerm)
    }

    data, err := ioutil.ReadFile(uhhConfig)

    if err != nil {
        log.Fatalf("%+v\n", err)
    }

    configLines := strings.Split(string(data), "\n")
    configValues := map[string]string{}

    for _, v := range configLines {
        keyValue := strings.Split(v, "=")
        if len(keyValue) == 2 {
            configValues[keyValue[0]] = keyValue[1]
        }
    }

    return types.Config{
        uhh,
        uhhConfig,
        repo,
        configValues,
    }
}

