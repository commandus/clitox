# clitox

Simple command line interface (CLI) TOX client.

- [Repository git@github.com:commandus/clitox.git](https://github.com/commandus/clitox.git)
- [Site https://github.com/commandus/clitox](https://github.com/commandus/clitox)

## Usage

clitox <Tox ID> 

For instance, 

```
clitox DE93C25176BBB999F6A18F44C163893561BF330C3CAC53FA07DF51A3F9C06E192EB9141124BB
```

will send messages to one client and


```
clitox DE93C25176BBB999F6A18F44C163893561BF330C3CAC53FA07DF51A3F9C06E192EB9141124BB 59E3FD36F1855FF729CBAFCA88913812ED7E314E54CA81E1F795BA42A5215D540274F8A17A49
```

will send messages to 2 clients both

### Stream

You can redirect or pipe stdin and stdout stream to other programs and scripts.

### Options

Show all options:

```
clitox --help
```

#### Use different configuration files

You can use the -f option to run utility with other Tox ID that saved in config file.
```
clitox -f tox_config.file
DE93C25176BBB999F6A18F44C163893561BF330C3CAC53FA07DF51A3F9C06E192EB9141124BB
```

By default clitox.tox file in current directory is used.

#### Print Tox ID

```
clitox -i
DE93C25176BBB999F6A18F44C163893561BF330C3CAC53FA07DF51A3F9C06E192EB9141124BB
```

### Options


#### Bootstrapping

If no bootstrapping nodes is set, hardcoded short list is used.

Get from Tox Bootstrap Nodes Status list [https://nodes.tox.chat/](https://nodes.tox.chat/json):
```
./clitox -b https://nodes.tox.chat/json 4542AF0241812A5B948B7F387D965D8E689AA5CED5E8B32E758E6264BE1D5E27F8C7B2E25C56 --local
```

or you can save nodes list in file and use downloaded file:

```
wget -o nodes.tox.chat.json https://nodes.tox.chat/json
./clitox -b nodes.tox.chat.json 4542AF0241812A5B948B7F387D965D8E689AA5CED5E8B32E758E6264BE1D5E27F8C7B2E25C56 --local
```


## Build

```
autoreconf -fi
automake --add-missing
./configure
make
sudo make install
```

### Windows

Refer to [TokTok/c-toxcore installation instructions] (https://github.com/TokTok/c-toxcore/blob/master/INSTALL.md#windows) how to obtain
dll, libs and headers for Visual Studio.

Open Visual Studio solution win/clitox/clitox.sln file, check Project properties.

Set include and lib path.
                                                                                        
You can download clitox.exe executable and libtox.dll (32bit) from the project releases.

Windows issues: client do not check and translate character codepage. You need set UTF-8 encoding in the console manually, or use pipes from/to programs working with UTF-8.


## Dependencies

- [c-toxcore](https://github.com/TokTok/c-toxcore)
- [The Sodium crypto library (libsodium)](https://github.com/jedisct1/libsodium)
- [argtable2](http://argtable.sourceforge.net/doc/argtable2.html)

Make each libraries static
```
./configure --enable-static --disable-shared
...
```
if you want.

