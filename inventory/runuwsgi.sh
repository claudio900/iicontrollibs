#!/bin/bash

pkill uwsgi
/usr/bin/uwsgi --emperor /usr/lib/iicontrollibs/inventory/ --daemonize /var/log/uwsgi.log