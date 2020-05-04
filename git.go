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

func gitPush(c *Config) bool {

	cmd := exec.Command("git",
		"-C",
		c.LocalRepoPath(),
		"push",
		"origin",
		"master",
	)

	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()

	return cmd.ProcessState.Success()
}

func gitAdd(c *Config) bool {
	cmd := exec.Command("git", "add", ".")
	cmd.Dir = c.LocalRepoPath()

	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()

	return cmd.ProcessState.Success()
}

func gitCommit(c *Config) bool {
	cmd := exec.Command("git",
		"commit",
		"-m",
		"updated at "+time.Now().String(),
	)

	cmd.Dir = c.LocalRepoPath()
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()

	return cmd.ProcessState.Success()
}

func gitPull(pathToRepo string) bool {
	cmd := exec.Command("git",
		"-C",
		pathToRepo,
		"pull",
		"--rebase",
	)
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()
	return cmd.ProcessState.Success()
}

func gitClone(repo string, path string) bool {
	cmd := exec.Command("git", "clone", repo, path)
	cmd.Stdout = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Run()
	return cmd.ProcessState.Success()
}
