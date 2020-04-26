package main

import (
	"log"
	"os"

	"github.com/theprimeagen/uhh"
	"github.com/urfave/cli"
	"golang.org/x/crypto/ssh/terminal"
)

func main() {
	cfg := getConfig()
	if !cloneConfigRepo(cfg) {
		log.Fatal("unable to get config")
	}

	uhh := uhh.New(cfg)
	ucli := newUhhCli(uhh)

	app := &cli.App{
		Name:   "uhh",
		Usage:  "find commands from your repo",
		Action: ucli.findHandler,
		Commands: []cli.Command{
			{Name: "add", Action: ucli.addHandler},
			{Name: "delete", Action: ucli.deleteHandler},
		},
	}
	app.Run(os.Args)
}

func readTermLine() string {
	oldState, err := terminal.MakeRaw(0)
	if err != nil {
		log.Fatal("could not change state of terminal")
	}
	defer terminal.Restore(0, oldState)
	t := terminal.NewTerminal(os.Stdin, ">")

	line, err := t.ReadLine()
	if err != nil {
		log.Fatal("unable to read line")
	}
	return line
}
