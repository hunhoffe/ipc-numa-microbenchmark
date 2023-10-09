# Results with attempting to control environment

## Baseline: Same Node (two runs: 0 then 1)
```
Server listen!
Server accept!
In second 0, ran 47168 iterations.
In second 1, ran 64768 iterations.
In second 2, ran 86400 iterations.
In second 3, ran 97472 iterations.
In second 4, ran 111040 iterations.
In second 5, ran 113600 iterations.
In second 6, ran 101312 iterations.
In second 7, ran 103232 iterations.
In second 8, ran 109056 iterations.
In second 9, ran 106944 iterations.
In second 10, ran 112256 iterations.
In second 11, ran 112960 iterations.
In second 12, ran 112896 iterations.
In second 13, ran 112768 iterations.
In second 14, ran 105024 iterations.
In second 15, ran 90752 iterations.
In second 16, ran 108736 iterations.
In second 17, ran 101248 iterations.
In second 18, ran 89984 iterations.
In second 19, ran 107136 iterations.
In second 20, ran 106048 iterations.
In second 21, ran 107456 iterations.
In second 22, ran 91008 iterations.
In second 23, ran 108288 iterations.
In second 24, ran 106496 iterations.
In second 25, ran 111232 iterations.
In second 26, ran 112384 iterations.
In second 27, ran 101184 iterations.
In second 28, ran 98304 iterations.
In second 29, ran 107392 iterations.
Total operations in the test is: 3044544.000000
Average operations per second is: 101484.796875
Exited successfully!
```

```
Client connect!
In second 0, ran 48768 iterations.
In second 1, ran 80384 iterations.
In second 2, ran 90048 iterations.
In second 3, ran 90112 iterations.
In second 4, ran 107648 iterations.
In second 5, ran 107648 iterations.
In second 6, ran 67392 iterations.
In second 7, ran 76544 iterations.
In second 8, ran 88064 iterations.
In second 9, ran 72640 iterations.
In second 10, ran 67328 iterations.
In second 11, ran 84096 iterations.
In second 12, ran 97344 iterations.
In second 13, ran 109888 iterations.
In second 14, ran 94528 iterations.
In second 15, ran 98176 iterations.
In second 16, ran 106560 iterations.
In second 17, ran 108032 iterations.
In second 18, ran 112896 iterations.
In second 19, ran 88384 iterations.
In second 20, ran 69312 iterations.
In second 21, ran 86464 iterations.
In second 22, ran 87488 iterations.
In second 23, ran 104320 iterations.
In second 24, ran 105280 iterations.
In second 25, ran 92224 iterations.
In second 26, ran 98176 iterations.
In second 27, ran 86912 iterations.
In second 28, ran 93504 iterations.
In second 29, ran 82752 iterations.
Total operations in the test is: 2702912.000000
Average operations per second is: 90097.070312
Exited successfully!
```

## Baseline: Different Nodes
```
Server listen!
Server accept!
In second 0, ran 34688 iterations.
In second 1, ran 50560 iterations.
In second 2, ran 58368 iterations.
In second 3, ran 67072 iterations.
In second 4, ran 70464 iterations.
In second 5, ran 70976 iterations.
In second 6, ran 71040 iterations.
In second 7, ran 70976 iterations.
In second 8, ran 71040 iterations.
In second 9, ran 71104 iterations.
In second 10, ran 71168 iterations.
In second 11, ran 64704 iterations.
In second 12, ran 63296 iterations.
In second 13, ran 65344 iterations.
In second 14, ran 68096 iterations.
In second 15, ran 64320 iterations.
In second 16, ran 63040 iterations.
In second 17, ran 65472 iterations.
In second 18, ran 68160 iterations.
In second 19, ran 70528 iterations.
In second 20, ran 65216 iterations.
In second 21, ran 62144 iterations.
In second 22, ran 65280 iterations.
In second 23, ran 68224 iterations.
In second 24, ran 70720 iterations.
In second 25, ran 71168 iterations.
In second 26, ran 67840 iterations.
In second 27, ran 62272 iterations.
In second 28, ran 65408 iterations.
In second 29, ran 68160 iterations.
Total operations in the test is: 1966848.000000
Average operations per second is: 65561.601562
Exited successfully!
```

## Baseline: AutoNUMA turned off (affinity not set)
```
Server listen!
Server accept!
In second 0, ran 48192 iterations.
In second 1, ran 74176 iterations.
In second 2, ran 77568 iterations.
In second 3, ran 99456 iterations.
In second 4, ran 95680 iterations.
In second 5, ran 94464 iterations.
In second 6, ran 109568 iterations.
In second 7, ran 107456 iterations.
In second 8, ran 86016 iterations.
In second 9, ran 104832 iterations.
In second 10, ran 107392 iterations.
In second 11, ran 108352 iterations.
In second 12, ran 97536 iterations.
In second 13, ran 97280 iterations.
In second 14, ran 110080 iterations.
In second 15, ran 91200 iterations.
In second 16, ran 108416 iterations.
In second 17, ran 109376 iterations.
In second 18, ran 109824 iterations.
In second 19, ran 113728 iterations.
In second 20, ran 113792 iterations.
In second 21, ran 113920 iterations.
In second 22, ran 113984 iterations.
In second 23, ran 114048 iterations.
In second 24, ran 114048 iterations.
In second 25, ran 100928 iterations.
In second 26, ran 95488 iterations.
In second 27, ran 104896 iterations.
In second 28, ran 81408 iterations.
In second 29, ran 55488 iterations.
Total operations in the test is: 2958592.000000
Average operations per second is: 98619.734375
Exited successfully!
```

## Baseline: AutoNUMA turned on (affinity not set)
```
Client connect!
In second 0, ran 48384 iterations.
In second 1, ran 76736 iterations.
In second 2, ran 83904 iterations.
In second 3, ran 102592 iterations.
In second 4, ran 107456 iterations.
In second 5, ran 104640 iterations.
In second 6, ran 97280 iterations.
In second 7, ran 106624 iterations.
In second 8, ran 87872 iterations.
In second 9, ran 107584 iterations.
In second 10, ran 109056 iterations.
In second 11, ran 93696 iterations.
In second 12, ran 98496 iterations.
In second 13, ran 107968 iterations.
In second 14, ran 90880 iterations.
In second 15, ran 60800 iterations.
In second 16, ran 83520 iterations.
In second 17, ran 93568 iterations.
In second 18, ran 108224 iterations.
In second 19, ran 110656 iterations.
In second 20, ran 110528 iterations.
In second 21, ran 110656 iterations.
In second 22, ran 110720 iterations.
In second 23, ran 110720 iterations.
In second 24, ran 110720 iterations.
In second 25, ran 87488 iterations.
In second 26, ran 88384 iterations.
In second 27, ran 83968 iterations.
In second 28, ran 103360 iterations.
In second 29, ran 106560 iterations.
Total operations in the test is: 2903040.000000
Average operations per second is: 96768.000000
Exited successfully!
```
