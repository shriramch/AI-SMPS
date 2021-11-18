#!/bin/bash

iter=0
cnt=0
maxv=-10000
bestP=0
bestD=0
bestC=0
bestL=0
bestM=0
bestF=0
rm log.txt inp.txt
while   [ $iter -le 5 ]
do
    P=$(($RANDOM % 10 + 10))
    D=$(($RANDOM % 10 + 10))
    C=$(($RANDOM % 500 + 500))
    L=$(($RANDOM % 500 + 500))
    M=$(($RANDOM % 50 + 50))
    F=$(($RANDOM % 50 + 50))
    echo "Cnt $cnt, Iter $iter: $P $D $C $L $M $F" >> log.txt
    echo "$P $D $C $L $M $F" > inp.txt
    output=$(./bin/Desdemona ./bots/MyBot/bot.so ./bots/RandomBot/RandomBot.so < inp.txt | tail -1)
    v=$output
    output=$(./bin/Desdemona ./bots/RandomBot/RandomBot.so ./bots/MyBot/bot.so < inp.txt| tail -1)
    v=$(($v-$output))
    output=$(./bin/Desdemona ./bots/MyBot/bot.so ./bots/RandomBot/RandomBot.so < inp.txt| tail -1)
    v=$(($v+$output))
    output=$(./bin/Desdemona ./bots/RandomBot/RandomBot.so ./bots/MyBot/bot.so < inp.txt| tail -1)
    v=$(($v-$output))
    echo $v >> log.txt
    if [ $v -gt $maxv ]
    then
        maxv=$v
        bestP=$P
        bestD=$D
        bestC=$C
        bestL=$L
        bestM=$M
        bestF=$F
        ((iter=0))
    else
        ((iter++))
    fi
    ((cnt++))
done

echo "Best: $bestP $bestD $bestC $bestL $bestM $bestF" >> log.txt