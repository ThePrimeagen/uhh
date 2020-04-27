package uhh

/*
import (
	"io/ioutil"
	"os"
	"reflect"
	"testing"
)

func TestReadConfig(t *testing.T) {
	cfgStr := `
	repo=https://github.com/theprimagen/uhh
	`
	f, closer, err := createTempCfg(cfgStr)
	defer closer()
	if err != nil {
		t.Error("unable to create temp file", err)
	}
	cfg, err := ReadConfig(f.Name())

	if err != nil {
		t.Error("un expected error")
	}

	if cfg.Repo() != "https://github.com/theprimagen/uhh" {
		t.Error("repo method returned unexpected value")
	}
}

func TestReadConfigWithoutRepo(t *testing.T) {
	cfgStr := `
	git=https://github.com/theprimagen/uhh
	`
	f, closer, err := createTempCfg(cfgStr)
	defer closer()
	if err != nil {
		t.Error("unable to create temp file", err)
	}

	cfg, err := ReadConfig(f.Name())
	if err != ErrRepoCfgNotFound {
		t.Error("expected reading config to fail with repo key not found but got", cfg, err)
	}
}

func createTempCfg(str string) (*os.File, func(), error) {
	f, err := ioutil.TempFile("", "without-repo")
	if err != nil {
		return nil, func() {}, err
	}

	f.WriteString(str)

	return f, func() { os.Remove(f.Name()) }, err
}
func TestConfigParser(t *testing.T) {
	inputs := []struct {
		name     string
		raw      string
		expected map[string]string
	}{
		{"positive test", "repo=https://github.com/theprimeagen/uhh-cfg", map[string]string{"repo": "https://github.com/theprimeagen/uhh-cfg"}},
		{"empty string parse", "", map[string]string{}},
		{"no equals sign", "this is a useless line", map[string]string{}},
		{"multiple equals signs", "woah=dude=where=is=my=car", map[string]string{"woah": "dude=where=is=my=car"}},
		{"space is ignored", "key = value", map[string]string{"key": "value"}},
	}

	for i := range inputs {
		tcase := inputs[i]
		t.Run(tcase.name, func(t *testing.T) {
			result, err := parseConfig(tcase.raw)
			if !reflect.DeepEqual(tcase.expected, result) {
				t.Errorf("parse result did not match for %s: expected: %+v, got: %+v", tcase.name, tcase.expected, result)
			}
		})
	}
}
*/
