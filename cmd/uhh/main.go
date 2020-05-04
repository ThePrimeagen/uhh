package main

import (
	"fmt"
	"io"
	"log"
	"os"
	"strings"

	"github.com/theprimeagen/uhh"
	"github.com/urfave/cli"
	"golang.org/x/crypto/ssh/terminal"
)

func main() {
	cfg, created := getConfig()

	uhh := uhh.New(cfg)
	ucli := newUhhCli(uhh)

	if created {
		err := uhh.Clone()

		if err != nil {
			log.Fatalf("%+v\n", err)
		}
	}

	app := &cli.App{
		Name:   "uhh",
		Usage:  "find commands from your repo",
		Action: ucli.findHandler,
		Commands: []cli.Command{
			{Name: "sync", Action: ucli.syncHandler},
			{Name: "add", Action: ucli.addHandler},
			{Name: "add-repo", Action: ucli.addRepoHandler},
			{Name: "delete", Action: ucli.deleteHandler},
		},
	}

	err := app.Run(os.Args)

	if err != nil {
		fmt.Printf("%+v\n", err)
		os.Exit(1)
	}
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
		if err != io.EOF {
			fmt.Fprintf(os.Stderr, "unable readline: %+v\n", err)
		}
		os.Exit(1)
	}

	return strings.TrimSpace(line)
}
