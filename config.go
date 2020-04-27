package uhh

import (
	"fmt"
	"io/ioutil"
	"os"
	"path"
	"encoding/json"
)

const repoKey = "repo"

var (
	ErrCfgNotFound     = fmt.Errorf("config file not found")
	ErrRepoCfgNotFound = fmt.Errorf("repo url not found in config")
)

type ConfigSpec struct {
    Repo *string `json:"repo"`
    SyncOnAdd bool `json:"syncOnAdd"`
    SyncOnAfterTime bool `json:"syncAfterTime"`
}

type Config struct {
	basePath      string
	configPath    string
	localRepoPath string
	vals *ConfigSpec
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

	values, err := parseConfig(rawConfig)
	basePath := path.Dir(configPath)
	localRepoPath := path.Join(basePath, "repo")

	return &Config{
		basePath:      basePath,
		configPath:    configPath,
		localRepoPath: localRepoPath,
		vals:          values,
	}, nil
}

func parseConfig(cfg []byte) (*ConfigSpec, error) {
    var spec ConfigSpec
    err := json.Unmarshal(cfg, &spec)

    return &spec, err
}

func CreateConfig(configPath, repo string) *Config {
	basePath := path.Dir(configPath)
	localRepoPath := path.Join(basePath, "repo")

	return &Config{
		configPath:    configPath,
		basePath:      basePath,
		localRepoPath: localRepoPath,
		vals:          &ConfigSpec{&repo, false, false},
	}
}

func (c *Config) Repo() string {
    if c.vals.Repo == nil {
        return ""
    }
	return *c.vals.Repo
}

func (c *Config) LocalRepoPath() string {
	return c.localRepoPath
}

func (c *Config) Path() string {
	return c.configPath
}

func (c *Config) Write(path string) error {
	data, err := json.Marshal(&c.vals)

    if err != nil {
        return err
    }

	return ioutil.WriteFile(path, data, os.ModePerm)
}
