
normie
------

``normie`` uses the ``liblognorm`` (https://github.com/rsyslog/liblognorm) to "normalize" data.  They typical
use case is to "normalize" log data.  For example, to extra usernames, IP address, hostnames, etc
for log files. 

``normie`` takes in data via ``stdin`` and outputs JSON objects of normalized data.  If specified, 
``normie`` will only output specific keys.  

To install ``normie`` do the following:

<pre>
$ sudo apt-get install liblognorm-dev
$ git clone https://github.com/quadrantsec/normie
$ cd normie
$ ./autogen   
$ ./configure
$ make
$ sudo make install
</pre>

``normie`` takes data in via ``stdin``.  This means that you can pipe data to it:

<pre>
$ cat /var/log/syslog | /usr/local/bin/normie /usr/local/etc/sagan-rules/normalization.rules
</pre>

``normie`` requires that you specify a "normalization rule based".  In this case, we are using
the Sagan Log Analysis Engine (https://github.com/quadrantsec/sagan) rule base file.  This file
is located at https://github.com/quadrantsec/sagan-rules (look for and download the 
``normalizations.rules" file). 

This file informs ``normie`` and ``liblognorm`` "how" to normalize log data.  For more information
about how ``liblognorm`` functions, see https://www.liblognorm.com/files/manual/index.html . 

Data that is successfully processed will be dumped to ``stdout`` in the form of a JSON blob, like
this:

<pre>
$ cut -d'|' -f9- /tmp/log  | /usr/local/bin/normie /usr/local/etc/sagan-rules/normalization.rulebase
{"src-port":"48832","src-ip":"192.168.1.134","username":"champ"}
{"src-port":"49254","src-ip":"192.168.1.11","username":"champ"}
{"src-port":"49714","src-ip":"192.168.1.15","username":"david"}
{"src-port":"38150","src-ip":"192.168.1.134","username":"steve"}
{"src-port":"38150","src-ip":"192.168.1.9","username":"frank"}
...
</pre>

In this particular case,  my data is pipe delimited and I'm using ``cut`` to extract only the 
syslog ``message`` field.  You will need to adjust your input to only be the data you want to 
normalize. 

In certain cases,  the JSON blob returned might be to much data.  For example,  you might want to
only normalize the "username" field.  That can be appended as a third options to ``normie``, like
this:

<pre>
$ cut -d'|' -f9- /tmp/log  | /usr/local/bin/normie /usr/local/etc/sagan-rules/normalization.rulebase username
champ
champ
david
steve
frank
...
</pre>

The idea is that you can use this tool to create small indexes of log data.  For example, lets 
say you have a directory of log files based on timestamp, like this:

<pre>
-rw-r--r-- 1 root root 297977 Dec 26 23:59 10.22.3.110-20221226.log.gz
-rw-r--r-- 1 root root 292392 Dec 27 23:59 10.22.3.110-20221227.log.gz
-rw-r--r-- 1 root root 295880 Dec 28 23:59 10.22.3.110-20221228.log.gz
-rw-r--r-- 1 root root 297221 Dec 29 23:59 10.22.3.110-20221229.log.gz
-rw-r--r-- 1 root root 229421 Dec 30 23:59 10.22.3.110-20221230.log.gz
-rw-r--r-- 1 root root 185357 Dec 31 23:59 10.22.3.110-20221231.log.gz
-rw-r--r-- 1 root root 184645 Jan  1 23:55 10.22.3.110-20230101.log.gz
-rw-r--r-- 1 root root 184402 Jan  2 23:55 10.22.3.110-20230102.log.gz
-rw-r--r-- 1 root root 182950 Jan  3 23:59 10.22.3.110-20230103.log.gz
-rw-r--r-- 1 root root 183591 Jan  4 23:55 10.22.3.110-20230104.log.gz
...
</pre>

Using ``normie``,  we can extract the "usernames" each file has in it. 

<pre>
$ for IN in *.gz; do zcat $IN |cut -d'|' -f9- | /usr/local/bin/normie /usr/local/etc/sagan-rules/normalization.rulesbase username > $IN.username.index; done
</pre>

This would parse all log files,  extract all "usernames" and store them in "index" files.  Like this:

<pre>
-rw-r--r-- 1 root root 297977 Dec 26 23:59 10.22.3.110-20221226.log.gz
-rw-r--r-- 1 root root    131 Dec 26 23:59 10.22.3.110-20221226.log.gz.username.index
-rw-r--r-- 1 root root 292392 Dec 27 23:59 10.22.3.110-20221227.log.gz
-rw-r--r-- 1 root root    231 Dec 27 23:59 10.22.3.110-20221227.log.gz.username.index
-rw-r--r-- 1 root root 295880 Dec 28 23:59 10.22.3.110-20221228.log.gz
-rw-r--r-- 1 root root    666 Dec 28 23:59 10.22.3.110-20221228.log.gz.username.index
...
</pre>

Now, when you want to search for a specified username from a text file, you can refer to the
index _first_ and only search the file that you _already know_ contact the username you are 
looking for.  For example:

<pre>
zgrep "champ" `grep "champ" *.username.index | cut -d':' -f1 | sed 's/\.username\.index//g'`
</pre>

This will potentially make searches fast since your are no longer searching files that we've
already determined do _not_ have the username you are looking for!

If you are looking to grab TCP/IP addresses from log files,  check out https://github.com/quadrantsec/grabip .  This tool does a better job of grabbing IP address and does not rely on ``liblognorm``.

