package uhh

import (
	"fmt"
	"io/ioutil"
	"os"
	"path"
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
	if !gitClone(u.config) {
		return fmt.Errorf("Unable to clone")
	}

	return nil
}

func (u *Uhh) Sync() error {
	origSnippets, err := ReadSnippetsFromDir(u.config.LocalRepoPath())
	if err != nil {
		return fmt.Errorf("unable to read snippets from dir: %w", err)
	}
	if !(gitAddAll(u.config) && gitStashPush(u.config)) {
		return fmt.Errorf("unable to stash files")
	}
	if !gitPull(u.config) {
		gitStashPop(u.config)
		return fmt.Errorf("unable fetch changes")
	}

	newSnippets, _ := ReadSnippetsFromDir(u.config.LocalRepoPath())

	diff := origSnippets.Diff(newSnippets)

	for _, d := range diff {
		u.Add(d.tag, d.cmd, d.searchTerms)
	}
	if !(gitAddAll(u.config) && gitCommit(u.config)) {
		return fmt.Errorf("Unable to commit")
	}

	if !gitPush(u.config) {
		return fmt.Errorf("Unable to commit")
	}

	return nil
}

func (u *Uhh) Find(tag string, searchTerms []string) (*FindResults, error) {

	// TODO: Multi repo support?
	tagPath := path.Join(u.config.LocalRepoPath(), tag)
	sns, err := ReadSnippetsFromFile(tagPath)
	if err != nil {
		fmt.Println("error:", err)
		return &FindResults{}, nil
	}

	findResults := FindResults{
		Tag: tag,
	}

	for _, s := range sns {
		contains := len(s.searchTerms) == 0

		for j := 0; j < len(searchTerms) && !contains; j++ {
			contains = strings.Contains(s.searchTerms, searchTerms[j])
		}

		if contains {
			findResults.Lines = append(findResults.Lines, s.line)
			findResults.Commands = append(findResults.Commands, s.cmd)
		}
	}

	return &findResults, nil
}

func New(cfg *Config) *Uhh {
	return &Uhh{cfg}
}

func (u *Uhh) Add(tag string, cmd string, searchTerms string) error {

	if contains(actions, tag) {
		return fmt.Errorf("You cannot have a tag as a reserverd word: %+v\n", actions)
	}

	tagPath := path.Join(u.config.LocalRepoPath(), tag)
	contents, err := ioutil.ReadFile(tagPath)

	if err != nil {
		contents = []byte{}
	}

	newContents := string(contents) + fmt.Sprintf("%s\n%s\n", cmd, searchTerms)
	ioutil.WriteFile(tagPath, []byte(newContents), os.ModePerm)

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
