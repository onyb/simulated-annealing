#!/bin/bash

function usage {
	cat <<- EOT
Simulated Annealing Algorithm

    Usage: ${0##/*/} [options]

    Options:
      -h|--help			Display this message and exit
      -p|--probability	Benchmark the success probabilities and plot graphs
      -t|--time         Benchmark the running time (real)

EOT
}

function setup {
    echo "Create directory: arXiv-1305-5837v1"
    mkdir -pv arXiv-1305-5837v1
    echo ""

    echo "Create directory: probability_graphs"
    mkdir -pv probability_graphs
    echo ""

    echo "Fetch Data Set: arXiv-1305-5837v1.tar.gz"
    wget http://gdurl.com/SpSM/download
    mv download arXiv-1305-5837v1.tar.gz
    echo ""

    echo "Extracting archive: arXiv-1305-5837v1.tar.gz"
    tar xf arXiv-1305-5837v1.tar.gz -C arXiv-1305-5837v1
    echo ""

    echo "Compile: anneal.c"
    gcc -O4 -Wno-unused-result -o anneal anneal.c -lm
    echo ""
}

function cleanup {
    echo "Cleaning up.."
    echo "Remove directory: arXiv-1305-5837v1"
    rm -Rf arXiv-1305-5837v1
    echo ""

    rm -f *~

    echo "Remove file: anneal"
    rm -f anneal
    echo ""

    echo "Done"
}

if [ "${1}" = "" ]; then
    echo "Error: No arguments passed"
    usage
fi

if [ "${#}" != "1" ]; then
    echo "Error: cannot handle multiple switches"
    usage
fi

PARAM=$(echo ${1} | awk -F= '{print $1}')

case ${PARAM} in
    -h | --help)
        usage
        ;;

    -p | --probability)
        setup
        for file in arXiv-1305-5837v1/anc/instances/*.txt ; do
            bash __anneal.sh $file
        done
        echo "Plotting graphs..."
        python plot.py
        echo ""
        cleanup
        ;;

    -t | --time)
        setup
        for file in arXiv-1305-5837v1/anc/instances/*.txt ; do
            echo "Run annealing with file: $file"
            runtime=`(time bash __anneal.sh $file) 2>&1 | grep real | sed "s|real ||g" | sed "s|s||g"`
            runtime="${runtime/*m/}"
            runtime=`echo $runtime 100 | awk '{printf "%f\n", $1/$2}'`
            echo "Running time: $runtime"
            echo ""
            echo "$file $runtime" >> arXiv-1305-5837v1/anc/runtime.csv
        done
        cleanup
        ;;

esac
