package main

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"time"

	"github.com/theprimeagen/uhh"
)

func getConfig() *uhh.Config {
	cfg, err := uhh.ReadUserConfig()
	if err == uhh.ErrCfgNotFound {
		cfg = readNewConfig()
		path, err := uhh.DefaultConfigPath()
		if err != nil {
			log.Fatal("getting default path for config failed", err)
		}
		cfg.Write(path)
	}

	return cfg
}

func readNewConfig() *uhh.Config {
	fmt.Println("Before Uhh can work, you need to provide a repo to save to!")
	fmt.Println("Please enter your repo meow: ")

	repoURL := readTermLine()

	defaultCfg, err := uhh.DefaultConfigPath()
	if err != nil {
		log.Fatal("unable to get default config path:", err)
	}
	return uhh.CreateConfig(defaultCfg, repoURL)
}

func cloneConfigRepo(c *uhh.Config) bool {
	if exists, shouldUpdate := checkLocalRepo(c.LocalRepoPath()); exists {
		if shouldUpdate {
			return gitFetch(c)
		}
		return true
	}
	return gitClone(c)
}

// we only say if we should update the repo every 6 hours
// todo: make this configurable?
func checkLocalRepo(path string) (exists bool, shouldUpdate bool) {
	finfo, err := os.Stat(path)
	if err != nil {
		return os.IsExist(err), false
	}
	shouldUpdate = finfo.ModTime().Before(time.Now().Add(-6 * time.Hour))
	return true, shouldUpdate
}

func gitFetch(c *uhh.Config) bool {
	cmd := exec.Command("git", "-C", c.LocalRepoPath(), "fetch")
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()
	return cmd.ProcessState.Success()

}
func gitClone(c *uhh.Config) bool {
	cmd := exec.Command("git", "clone", c.Repo(), c.LocalRepoPath())
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()
	return cmd.ProcessState.Success()
}
