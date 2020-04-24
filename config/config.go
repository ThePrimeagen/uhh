package config

import (
    "io/ioutil"
    "strings"
    "path"
    "golang.org/x/crypto/ssh/terminal"
    "os"
    "os/exec"
    "log"
    "fmt"
)

type UhhConfig struct {
    Uhh string
    Config string
    Repo string
    Values map[string]string
}

func readConfig() {
    dat, err := ioutil.ReadFile("/tmp/dat")

    if err != nil {
        log.Fatalf("%+v\n", err)
    }

    fmt.Print(string(dat))
}

func InitConfig()  UhhConfig {
    config, err := os.UserConfigDir()

    if err != nil {
        log.Fatalf("%+v\n", err)
    }

    uhh := path.Join(config, "uhh")
    repo := path.Join(uhh, "repo")
    uhhConfig := path.Join(uhh, ".config")

    fmt.Printf("Uhh %s\nRepo %s\nuhhConfig %s\n", uhh, repo, uhhConfig);

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

    return UhhConfig{
        uhh,
        uhhConfig,
        repo,
        configValues,
    }
}

