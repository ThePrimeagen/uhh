package main

import (
	"fmt"
	"log"

	"github.com/theprimeagen/uhh"
)

func getConfig() (*uhh.Config, bool) {
	cfg, err := uhh.ReadUserConfig()

	created := false

	if err == uhh.ErrCfgNotFound {

		cfg = readNewConfig()
		created = cfg.Repo() != ""
		path, err := uhh.DefaultConfigPath()
		if err != nil {
			log.Fatal("getting default path for config failed", err)
		}

		cfg.Save(path)
	}

	return cfg, created
}

func readNewConfig() *uhh.Config {
	fmt.Println("Before you get started on Uhh, do you have a repo you would like to save your commands too?")
	fmt.Println("Please enter your repo meow (empty = no): ")

	repoURL := readTermLine()

	defaultCfg, err := uhh.DefaultConfigPath()
	if err != nil {
		log.Fatal("unable to get default config path:", err)
	}

	return uhh.CreateConfig(defaultCfg, repoURL)
}
