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

func (u *Uhh) Find(tag string, searchTerms []string) (*FindResults, error) {

	// TODO: Multi repo support?
	tagPath := path.Join(u.config.LocalRepoPath(), tag)

	fileContents, err := ioutil.ReadFile(tagPath)

	if err != nil {
		return &FindResults{}, nil
	}

	findResults := FindResults{
		Tag: tag,
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
			findResults.Lines = append(findResults.Lines, i)
			findResults.Commands = append(findResults.Commands, command)
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
