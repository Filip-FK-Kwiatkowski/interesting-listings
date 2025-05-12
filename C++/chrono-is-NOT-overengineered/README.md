The output of the program is:

```
2019-03-20 18:00:00 CET
2019-03-20 13:00:00 EDT
2019-04-03 18:00:00 CEST
2019-04-03 12:00:00 EDT
```

Which means that `std::chrono` correctly handled the inconsistent timezone changes.
