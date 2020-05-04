package uhh

import (
	"fmt"
	"io/ioutil"
	"os"
	"path"
	"strconv"
	"strings"
)

type Uhh struct {
	config *Config
}

type FindResults struct {
	Tag      string
	Commands []string
	Lines    []int
}

func contains(arr []string, str string) bool {
	for _, a := range arr {
		if a == str {
			return true
		}
	}
	return false
}

func (u *Uhh) Clone() error {
	if !gitClone(u.config.Repo(), u.config.LocalRepoPath()) {
		return fmt.Errorf("Unable to clone")
	}

	return nil
}

func (u *Uhh) AddRepo(repoUrl string) error {
	idx := len(u.config.vals.ReadRepos)
	repoPath := u.config.GetReadRepoPath(idx)

	if !gitClone(repoUrl, repoPath) {
		return fmt.Errorf("Unable to clone")
	}

	u.config.vals.ReadRepos = append(u.config.vals.ReadRepos, strconv.Itoa(idx))

	path, err := DefaultConfigPath()

	if err != nil {
		os.RemoveAll(repoPath)
		return fmt.Errorf("%w", err)
	}

	err = u.config.Save(path)

	if err != nil {
		os.RemoveAll(repoPath)
		return fmt.Errorf("%w", err)
	}

	return nil
}

func (u *Uhh) SetRepo() error {
	return fmt.Errorf("Definitely not a JavaException here: NotImplemented")
}

func (u *Uhh) Sync() error {
	if !gitAdd(u.config) || !gitCommit(u.config) {
		return fmt.Errorf("Unable to commit")
	}

	if !gitPush(u.config) {
		return fmt.Errorf("Unable to commit")
	}

	for i := range u.config.ReadRepos() {
		if !gitPull(u.config.GetReadRepoPath(i)) {
			return fmt.Errorf("Unable to pull from repo %d", i)
		}
	}

	return nil
}

func find(tagPath string, searchTerms []string, results *FindResults) error {
	fileContents, err := ioutil.ReadFile(tagPath)

	if err != nil {
		return nil
	}

	contents := strings.Split(string(fileContents), "\n")

	for i := 1; i < len(contents); i = i + 2 {
		command := contents[i-1]
		searchTokens := contents[i]
		contains := len(searchTerms) == 0

		for j := 0; j < len(searchTerms) && !contains; j++ {
			contains = strings.Contains(searchTokens, searchTerms[j])
		}

		if contains {
			results.Lines = append(results.Lines, i)
			results.Commands = append(results.Commands, command)
		}
	}

	return nil
}

func (u *Uhh) Find(tag string, searchTerms []string) (*FindResults, error) {
	results := FindResults{
		Tag: tag,
	}
	tagPath := path.Join(u.config.LocalRepoPath(), tag)
	err := find(tagPath, searchTerms, &results)

	if err != nil {
		return nil, fmt.Errorf("%w\n", err)
	}

	for i := range u.config.ReadRepos() {
		tagPath = path.Join(u.config.GetReadRepoPath(i), tag)
		err := find(tagPath, searchTerms, &results)

		if err != nil {
			return nil, fmt.Errorf("%w\n", err)
		}
	}

	return &results, nil
}

func New(cfg *Config) *Uhh {
	return &Uhh{cfg}
}

func (u *Uhh) Add(tag string, cmd string, searchTerms string) error {

	if contains(actions, tag) {
		return fmt.Errorf("You cannot have a tag as a reserverd word: %+v\n", actions)
	}

	if u.config.vals.SyncOnAdd {
		if !gitPull(u.config.LocalRepoPath()) {
			return fmt.Errorf("Unable to pull from repo")
		}
	}

	tagPath := path.Join(u.config.LocalRepoPath(), tag)
	contents, err := ioutil.ReadFile(tagPath)

	if err != nil {
		contents = []byte{}
	}

	newContents := string(contents) + fmt.Sprintf("%s\n%s\n", cmd, searchTerms)
	err = ioutil.WriteFile(tagPath, []byte(newContents), os.ModePerm)

	if err != nil {
		return fmt.Errorf("Unable to write the command and search terms: %w\n", err)
	}

	if u.config.vals.SyncOnAdd {
		if !gitAdd(u.config) || !gitCommit(u.config) {
			return fmt.Errorf("Unable to commit")
		}
		if !gitPush(u.config) {
			return fmt.Errorf("Unable push")
		}
	}

	return nil
}

func (u *Uhh) Delete(results *FindResults) error {
	tagPath := path.Join(u.config.LocalRepoPath(), results.Tag)
	tagFileContents, err := ioutil.ReadFile(tagPath)

	if err != nil {
		return fmt.Errorf("Unable to read %s: %w", tagPath, err)
	}

	tagFile := strings.Split(string(tagFileContents), "\n")
	for i := len(results.Lines) - 1; i >= 0; i-- {
		tagFile = append(tagFile[0:i], tagFile[i+2:]...)
	}

	err = ioutil.WriteFile(
		tagPath, []byte(strings.Join(tagFile, "\n")), os.ModePerm)

	if err != nil {
		return fmt.Errorf("Unable to write file with new tagContents: %w\n", err)
	}

	return nil
}
