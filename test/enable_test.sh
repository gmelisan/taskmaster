#!/bin/bash

if [ "${1}" == "" ]; then
	echo Pass test number as argument
	exit 1
fi

CURRENT_DIR=$(cd `dirname $0` && pwd)
FILE="${CURRENT_DIR}/${1}.yml"

if [ -f "${FILE}" ]; then
	cat "${FILE}" > "${CURRENT_DIR}/taskmaster-config.yml"
else
	echo "${FILE} does not exist."
fi
