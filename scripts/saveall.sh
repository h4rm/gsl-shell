#!/bin/bash

#tar czvf $1 `find . -type f -not -regex '.*\.\(o\|exe\|dll\|a\|so\)' -a -not -regex '.*\.\(svn\|libs\|deps\).*' -not -name '*~' -a -not -regex '\(html|lua\)'`
tar czvf $1 `find . -type f -regex '.*\.\(c\|cpp\|lua\)' -or -iname 'make*'`