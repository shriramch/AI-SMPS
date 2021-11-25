#!/bin/bash

iter=0
cnt=0
maxv=-100000000
bestP=0
bestD0=0
bestD1=0
bestD2=0
bestC=0
bestL=0
bestM=0
bestF=0
rm log.txt inp.txt
while   [ $iter -ge 0 ]
do
    P=$(($RANDOM % 20 + 1))
    D0=$(($RANDOM % 20 + 1))
    D1=$(($RANDOM % 20 + 1))
    D2=$(($RANDOM % 20 + 1))
    C=$(($RANDOM % 600 + 401))
    L=$(($RANDOM % 600 + 401))
    M=$(($RANDOM % 60 + 41))
    F=$(($RANDOM % 60 + 41))
    echo "Cnt $cnt, Iter $iter: $P $D0 $D1 $D2 $C $L $M $F" >> log.txt
    echo "$P $D0 $D1 $D2 $C $L $M $F" > inp.txt
    output=$(./bin/Desdemona ./bots/MyBot/botNice.so ./bots/Bots/0.so < inp.txt | tail -1)
    v=$output
    output=$(./bin/Desdemona ./bots/MyBot/botNice.so ./bots/Bots/2.so < inp.txt | tail -1)
    v=$(($v+$output))
    output=$(./bin/Desdemona ./bots/MyBot/botNice.so ./bots/Bots/4.so < inp.txt | tail -1)
    v=$(($v+$output))
    output=$(./bin/Desdemona ./bots/MyBot/botNice.so ./bots/Bots/6.so < inp.txt | tail -1)
    v=$(($v+$output))
    output=$(./bin/Desdemona ./bots/MyBot/botNice.so ./bots/Bots/8.so < inp.txt | tail -1)
    v=$(($v+$output))
    output=$(./bin/Desdemona ./bots/MyBot/botNice.so ./bots/Bots/bapat.so < inp.txt | tail -1)
    v=$(($v+$output))
    output=$(./bin/Desdemona ./bots/Bots/1.so ./bots/MyBot/botNice.so < inp.txt| tail -1)
    v=$(($v-$output))
    output=$(./bin/Desdemona ./bots/Bots/3.so ./bots/MyBot/botNice.so < inp.txt| tail -1)
    v=$(($v-$output))
    output=$(./bin/Desdemona ./bots/Bots/5.so ./bots/MyBot/botNice.so < inp.txt| tail -1)
    v=$(($v-$output))
    output=$(./bin/Desdemona ./bots/Bots/7.so ./bots/MyBot/botNice.so < inp.txt| tail -1)
    v=$(($v-$output))
    output=$(./bin/Desdemona ./bots/Bots/9.so ./bots/MyBot/botNice.so < inp.txt| tail -1)
    v=$(($v-$output))
    output=$(./bin/Desdemona ./bots/Bots/fake.so ./bots/MyBot/botNice.so < inp.txt| tail -1)
    v=$(($v-$output))
    echo $v >> log.txt
    if [ $v -lt $maxv ]
    then
        maxv=$v
        bestP=$P
        bestD0=$D0
        bestD1=$D1
        bestD2=$D2
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

echo "Best: $bestP $bestD0 $bestD1 $bestD2 $bestC $bestL $bestM $bestF" >> log.txt