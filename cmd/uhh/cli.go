package main

import (
	"fmt"
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

func (ucli *uhhCli) syncHandler(c *cli.Context) error {
	return ucli.backend.Sync()
}

func (ucli *uhhCli) findHandler(c *cli.Context) error {

	if c.Args().Present() {
		results, err := ucli.backend.Find(c.Args().First(), c.Args().Tail())

		if err != nil {
			return fmt.Errorf("Error from find: %w", err)
		}

		fmt.Printf("%s", strings.Join(results.Commands, "\n"))

		return nil
	}

	cli.ShowAppHelp(c)

	return nil
}

func (ucli *uhhCli) addHandler(c *cli.Context) error {
	var tag string
	if c.Args().Present() {
		tag = c.Args().First()
	} else {
		fmt.Println("tag name")
		tag = readTermLine()
	}

	fmt.Println("please enter command")
	cmd := readTermLine()

	fmt.Println("please enter space delimited search terms")
	jointTerms := readTermLine()

	err := ucli.backend.Add(tag, cmd, jointTerms)

	if err != nil {
		return fmt.Errorf("unable to add command: %w", err)
	}

	return nil
}

func (ucli *uhhCli) deleteHandler(c *cli.Context) error {

	if !c.Args().Present() {
		return fmt.Errorf("Must provide tag and optional search terms to delete.")
	}

	results, err := ucli.backend.Find(c.Args().First(), c.Args().Tail())

	if err != nil {
		return err
	}

	if len(results.Commands) == 0 {
		return fmt.Errorf("Unable to find any commands to delete")
	}

	fmt.Printf("About to delete %d items.\n", len(results.Commands))
	for i, value := range results.Commands {
		fmt.Printf("%d: %s\n", i, value)
	}

	fmt.Printf("Confirm delete (y/N): ")
	line := readTermLine()

	if len(line) != 1 || line[0] != 'y' && line[0] != 'Y' {
		return nil
	}

	ucli.backend.Delete(results)

	return nil
}
