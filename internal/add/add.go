package add

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path"
	"bufio"

	"github.com/theprimeagen/uhh/internal/constants"
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

func Add(config *types.Config, args []string) {
    var tag string

    reader := bufio.NewReader(os.Stdin)
if len(args) > 0 {
        tag = args[0]
    } else {

        fmt.Printf("Please enter the tag: ")

        line, err := reader.ReadString('\n')

        if err != nil {
            log.Fatalf("%+v\n", err)
        }

        if len(line) == 0 {
            log.Fatalf("You did not provide a tag, therefore we must quit.\n", err)
        }

        tag = line
    }

    fmt.Printf("Please enter the command: ")
    command, err := reader.ReadString('\n')

    if err != nil {
        log.Fatalf("%+v\n", err)
    }

    if len(command) == 0 {
        log.Fatalf("You cannot provide an empty command\n")
    }

    if contains(constants.Actions, command) {
        log.Fatalf("You cannot name your command the same as reserved words: %+v\n")
    }

    fmt.Printf("Please enter the search terms: ")
    searchTerms, err := reader.ReadString('\n')

    tagPath := path.Join(config.Repo, tag)
    contents, err := ioutil.ReadFile(tagPath)

    if err != nil {
        contents = []byte{}
    }

    newContents := string(contents) + fmt.Sprintf("%s%s", command, searchTerms);
    ioutil.WriteFile(tagPath[0:len(tagPath) - 1], []byte(newContents), os.ModePerm)
}

