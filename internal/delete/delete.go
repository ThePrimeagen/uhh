package delete

import (
	"strings"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path"
	"bufio"

	"github.com/theprimeagen/uhh/internal/find"
	"github.com/theprimeagen/uhh/internal/types"
)

func Delete(config *types.Config, args []string) {
    results := find.Find(config, args)

    if len(results.Commands) == 0 {
        fmt.Printf("Unable to find any commands to delete")
        return
    }

    fmt.Printf("About to delete %d items.\n", len(results.Commands))
    for i, value := range results.Commands {
        fmt.Printf("%d: %s\n", i, value)
    }

    fmt.Printf("Confirm delete (y/n): ")
    reader := bufio.NewReader(os.Stdin)
    line, err := reader.ReadString('\n')

    if err != nil {
        log.Fatalf("%+v\n", err)
    }

    if line[0] != 'y' {
        return;
    }

    tagPath := path.Join(config.Repo, args[0])
    tagFileContents, err := ioutil.ReadFile(tagPath)

    if err != nil {
        log.Fatalf("%+v\n", err)
    }

    tagFile := strings.Split(string(tagFileContents), "\n")
    for i := len(results.Lines) - 1; i >= 0; i-- {
        tagFile = append(tagFile[0:i], tagFile[i + 2:]...)
    }

    err = ioutil.WriteFile(
        tagPath, []byte(strings.Join(tagFile, "\n")), os.ModePerm)

    if err != nil {
        log.Fatalf("Unable to write file with new tagContents: %+v\n", err)
    }

}




