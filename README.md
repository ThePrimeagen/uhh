# uhh
When you keep forgetting those sweet sweet sweet sweet commands.

# Requirements

# How to build
TODO:

### Usage

#### Directories
Uhh looks at the environment variables `UHH_DIR` and `XDG_CONFIG_HOME` to determine
where it should put its files on its first run. `UHH_DIR` should be an already
existing folder where you wish all of Uhh's resources to be put, while
`XDG_CONFIG_HOME` should be the parent folder where Uhh will make a folder for
itself called `uhh`.

If neither of these are defined, Uhh defaults to `$HOME/.config/uhh`.

#### Basic Usage
`uhh [options] {cmd/token} {searching tokens}`

```
uhh uhh = help
uhh wat = help
uhh huh = help
    Prints the help menu

uhh list = list
uhh what = list
    list all the categories

uhh update
    from repo: git pull origin master

uhh delete {cmd} {tokens for searching}
    How does this work?
    list categories, you put in number
    list items in category, you put in number
    are you sure?

uhh add
    cat: onteuhnoteuhnoteuh
    one-line desc: onetuhonteuhonetonetuhonetuh
    cmd: eontuhoentuhonteuc:w

uhh sync
    Syncs the repo (git commit -m 'ontehunoteh' && git push origin master)

uhh tcpdump
 "One line explanation"
 tcpdump -....

uhh -F
  "onteuhone"
  perf -F
```


