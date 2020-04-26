package main

import (
	"fmt"
	"log"
	"strings"

	"github.com/theprimeagen/uhh"
	"github.com/urfave/cli"
)

type uhhCli struct {
	backend *uhh.Uhh
}

func newUhhCli(backend *uhh.Uhh) *uhhCli {
	return &uhhCli{
		backend: backend,
	}
}

func (ucli *uhhCli) findHandler(c *cli.Context) error {
	if c.Args().Present() {
		results := ucli.backend.Find(c.Args().First(), c.Args().Tail())
		for _, r := range results {
			fmt.Printf("%s - %s\n", r.Name, strings.Join(r.Terms, ", "))
		}
		return nil
	}
	cli.ShowAppHelp(c)
	return nil
}

func (ucli *uhhCli) addHandler(c *cli.Context) error {
	fmt.Println("add command")
	cmd := readTermLine()
	fmt.Println("please enter comma separated search terms")
	jointTerms := readTermLine()

	terms := strings.Split(jointTerms, ",")

	err := ucli.backend.Add(cmd, terms)
	if err != nil {
		log.Fatal("unable to add command:", err)
	}
	return nil
}

func (ucli *uhhCli) deleteHandler(c *cli.Context) error {
	return nil
}
