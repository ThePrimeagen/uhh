package uhh

import (
	"fmt"
	"io/ioutil"
	"path"
	"path/filepath"
	"strings"
)

type Snippet struct {
	line        int
	tag         string
	cmd         string
	searchTerms string
}

type Snippets []*Snippet

func (sns Snippets) Diff(other Snippets) Snippets {
	dummy := struct{}{}

	lookupTable := make(map[Snippet]struct{}, len(sns))
	for _, s := range other {
		lookupTable[*s] = dummy
	}

	result := []*Snippet{}

	for _, s := range sns {
		if _, ok := lookupTable[*s]; !ok {
			result = append(result, s)
		}
	}
	return result
}

func ReadSnippetsFromDir(loc string) (Snippets, error) {
	files, err := ioutil.ReadDir(loc)
	if err != nil {
		return nil, fmt.Errorf("unable to read dir: %w", err)
	}
	result := []*Snippet{}
	for _, f := range files {
		if f.IsDir() {
			// we only read files :)
			continue
		}
		snippets, err := ReadSnippetsFromFile(path.Join(loc, f.Name()))
		if err != nil {
			fmt.Println("unable to read file", f.Name())
			continue
		}
		result = append(result, snippets...)
	}

	return result, nil
}

func ReadSnippetsFromFile(file string) (Snippets, error) {
	_, tag := filepath.Split(file)

	contents, err := ioutil.ReadFile(file)
	if err != nil {
		return nil, fmt.Errorf("unable to read file: %w", err)
	}

	lines := strings.Split(strings.TrimSpace(string(contents)), "\n")

	if len(lines)%2 != 0 {
		return nil, fmt.Errorf("invalid tag files, dones not contain expected amout of lines: %d", len(lines))
	}

	result := []*Snippet{}
	for i := 1; i < len(lines); i = i + 2 {
		command := lines[i-1]
		searchTerms := lines[i]

		result = append(result, &Snippet{
			line:        i,
			tag:         tag,
			cmd:         command,
			searchTerms: searchTerms,
		})
	}
	return result, nil
}
