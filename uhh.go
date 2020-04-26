package uhh

import "fmt"

type Uhh struct {
	entries []*CmdEntry
}

func New(cfg *Config) *Uhh {
	return &Uhh{
		entries: []*CmdEntry{
			newEntry("hmm", "uhh"),
			newEntry("rust", "yaya"),
		},
	}
}

func (u *Uhh) Find(tag string, desc []string) []*CmdEntry {
	return u.entries
}

func (u *Uhh) Add(cmd string, searchTerms []string) error {
	fmt.Println("adding command", cmd)
	return fmt.Errorf("not implemented")
}
func (u *Uhh) Delete(entry *CmdEntry) error {
	if entry.line == -1 || entry.file == "<mem>" {
		return fmt.Errorf("remove demo entries and point to file line location")
	}
	return fmt.Errorf("not implemented")
}

type CmdEntry struct {
	Name  string
	Terms []string

	line int
	file string
}

func newEntry(name string, terms ...string) *CmdEntry {
	return &CmdEntry{
		Name:  name,
		Terms: terms,
		file:  "<mem>",
		line:  -1,
	}
}
