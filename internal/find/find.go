package find

import (
	"fmt"
	"io/ioutil"
	"log"
	"path"
	"strings"

	"github.com/theprimeagen/uhh/internal/types"
	// "golang.org/x/crypto/ssh/terminal"
)

func contains(arr []string, str string) bool {
   for _, a := range arr {
      if a == str {
         return true
      }
   }
   return false
}

type FindResults struct {
    Commands []string
    Lines []int
}

func Find(config *types.Config, args []string) FindResults {

    if len(args) == 0 {
        log.Fatalf("You must provide a tag and \n")
    }

    tag := args[0]
    var searchTerms []string
    if len(args) > 1 {
        searchTerms = args[1:]
    } else {
        searchTerms = []string{}
    }

    tagPath := path.Join(config.Repo, tag)
    fileContents, err := ioutil.ReadFile(tagPath)

    if err != nil {
        return FindResults{}
    }

    var findResults FindResults

    contents := strings.Split(string(fileContents), "\n")

    for i := 1; i < len(contents); i = i + 2 {
        command := contents[i - 1];
        searchTokens := contents[i];
        contains := len(searchTerms) == 0

        for j := 0; j < len(searchTerms) && !contains; j++ {
            contains = strings.Contains(searchTokens, searchTerms[j])
        }

        if contains {
            findResults.Lines = append(findResults.Lines, i)
            findResults.Commands = append(findResults.Commands, command)
        }
    }

    return findResults
}

func FindAndPrint(config *types.Config, args []string) {
    results := Find(config, args)

    if len(results.Commands) == 0 {
        fmt.Printf("Nothing Found");
        return
    }

    for _, value := range results.Commands {
        fmt.Printf("%s", value)
    }
}

