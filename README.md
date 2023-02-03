
normie
------

``normie`` uses the ``liblognorm`` (https://www.liblognorm.com/) to "normalize" data.  They typical
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


