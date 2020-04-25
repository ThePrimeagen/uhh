package uhh

import (
	"fmt"
	"io/ioutil"
	"os"
	"path"
	"strings"
)

const repoKey = "repo"

var (
	ErrCfgNotFound     = fmt.Errorf("config file not found")
	ErrRepoCfgNotFound = fmt.Errorf("repo url not found in config")
)

type Config struct {
	basePath      string
	configPath    string
	localRepoPath string
	vals          map[string]string
}

func DefaultConfigPath() (string, error) {
	cfgDir, err := os.UserConfigDir()
	if err != nil {
		return "", fmt.Errorf("can'd find the default home dir: %w", err)
	}

	basePath := path.Join(cfgDir, "uhh")

	err = os.MkdirAll(cfgDir, os.ModePerm)
	if err != nil {
		return "", fmt.Errorf("unable to create uhh config dir '%s': %w", basePath, err)
	}

	return path.Join(basePath, ".config"), nil
}

func ReadUserConfig() (*Config, error) {
	cfgPath, err := DefaultConfigPath()
	if err != nil {
		return nil, fmt.Errorf("unable to get default config path: %w", err)
	}
	return ReadConfig(cfgPath)
}

func ReadConfig(configPath string) (*Config, error) {
	rawConfig, err := ioutil.ReadFile(configPath)
	if os.IsNotExist(err) {
		return nil, ErrCfgNotFound
	}

	values := parseConfig(string(rawConfig))

	if _, ok := values[repoKey]; !ok {
		return nil, ErrRepoCfgNotFound
	}

	basePath := path.Dir(configPath)
	localRepoPath := path.Join(basePath, "repo")

	return &Config{
		basePath:      basePath,
		configPath:    configPath,
		localRepoPath: localRepoPath,
		vals:          values,
	}, nil
}

func parseConfig(cfg string) map[string]string {
	lines := strings.Split(cfg, "\n")

	cfgMap := map[string]string{}
	for _, l := range lines {
		kv := strings.SplitN(l, "=", 2)
		if len(kv) == 2 {
			cfgMap[strings.TrimSpace(kv[0])] = strings.TrimSpace(kv[1])
		}
	}

	return cfgMap
}

func CreateConfig(configPath, repo string) *Config {
	basePath := path.Dir(configPath)
	localRepoPath := path.Join(basePath, "repo")

	return &Config{
		configPath:    configPath,
		basePath:      basePath,
		localRepoPath: localRepoPath,
		vals:          map[string]string{repoKey: repo},
	}
}

func (c *Config) Keys() []string {
	keys := []string{}
	for k := range c.vals {
		keys = append(keys, k)
	}
	return keys
}

func (c *Config) Value(k string) (string, bool) {
	v, ok := c.vals[k]
	return v, ok
}

func (c *Config) Repo() string {
	return c.vals[repoKey]
}

func (c *Config) LocalRepoPath() string {
	return c.localRepoPath
}

func (c *Config) Path() string {
	return c.configPath
}

func (c *Config) Write(path string) error {
	data := c.serialize()
	return ioutil.WriteFile(path, data, os.ModePerm)
}

func (c *Config) serialize() []byte {
	var out strings.Builder
	for k, v := range c.vals {
		out.WriteString(k + "=" + v + "\n")
	}
	return []byte(out.String())
}
