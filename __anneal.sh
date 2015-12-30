#! /bin/bash

energy=$(cat $1 | head -1)
sed -i "s|$energy|${energy/* }|g" $1
count=1
success=0
echo "Run annealing with file: $1"
while [ $count != 100 ]; do
    if [ $(./anneal < $1) == "SUCCESS" ]; then
        ((success++))
    fi
    ((count++))
done

echo "Success probability: $success"
echo ""
echo $success >> arXiv-1305-5837v1/anc/predicted_success.csv