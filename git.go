package uhh

import (
	"os"
	"os/exec"
	"time"
)

func gitFetch(c *Config) bool {
	cmd := exec.Command("git", "-C", c.LocalRepoPath(), "fetch")
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()
	return cmd.ProcessState.Success()

}

func gitPull(c *Config) bool {
	cmd := exec.Command("git",
		"-C",
		c.LocalRepoPath(),
		"pull",
		"--rebase",
	)

	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()

	return cmd.ProcessState.Success()
}

func gitPush(c *Config) bool {

	cmd := exec.Command("git",
		"-C",
		c.LocalRepoPath(),
		"push",
	)

	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()

	return cmd.ProcessState.Success()
}

func gitCommit(c *Config) bool {

	cmd := exec.Command("git",
		"-C",
		c.LocalRepoPath(),
		"commit",
		"-am",
		"updated at "+time.Now().String(),
	)

	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()

	return cmd.ProcessState.Success()
}

func gitAddAll(c *Config) bool {
	cmd := exec.Command("git",
		"-C",
		c.LocalRepoPath(),
		"add",
		".",
	)
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()
	return cmd.ProcessState.Success()
}

func gitAdd(c *Config) bool {
	cmd := exec.Command("git", "clone", c.Repo(), c.LocalRepoPath())
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()
	return cmd.ProcessState.Success()
}

func gitClone(c *Config) bool {
	cmd := exec.Command("git", "clone", c.Repo(), c.LocalRepoPath())
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()
	return cmd.ProcessState.Success()
}

func gitStashPush(c *Config) bool {
	cmd := exec.Command("git",
		"-C",
		c.LocalRepoPath(),
		"stash",
	)

	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()

	return cmd.ProcessState.Success()
}

func gitStashPop(c *Config) bool {
	cmd := exec.Command("git",
		"-C",
		c.LocalRepoPath(),
		"stash",
		"pop",
	)

	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()

	return cmd.ProcessState.Success()
}
