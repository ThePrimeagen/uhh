package types

type UhhConfig struct {
    Uhh string
    Config string
    Repo string
    Values map[string]string
}


type CmdFunc = func(conf *UhhConfig, args []string)



