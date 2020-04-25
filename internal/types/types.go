package types

type Config struct {
    Uhh string
    Config string
    Repo string
    Values map[string]string
}

type CmdFunc = func(conf *Config, args []string)
