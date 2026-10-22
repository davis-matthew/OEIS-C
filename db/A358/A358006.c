/*
 * A358006  Number of partitions of n into 6 distinct positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A358006
 *
 * Offset:    32    (a(32) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 6 distinct Fibonacci
 *            numbers * (the Fibonacci set is {1,2,3,5,8,13,…} – the duplicate
 *            1 is treated only once)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t; idx_t is supplied by the build */

seq_t a(idx_t n)
{
    /* For n < 32 no partition can exist because the smallest 6 distinct
       Fibonacci numbers sum to 32: 1+2+3+5+8+13 = 32. */
    if (n < 32)
        return 0;

    /* Generate all Fibonacci numbers <= n.  We use the variation of the
       Fibonacci sequence that starts 1, 2, 3, … to avoid the duplicate 1. */
    seq_t fib[200];      /* 200 is more than enough for n up to 10^100 or so */
    idx_t fibcnt = 0;

    fib[fibcnt++] = 1;
    fib[fibcnt++] = 2;
    while (1) {
        seq_t next = fib[fibcnt - 1] + fib[fibcnt - 2];
        if (next > n)
            break;
        fib[fibcnt++] = next;
    }

    /* If fewer than 6 Fibonacci numbers are available, no partition exists. */
    if (fibcnt < 6)
        return 0;

    /* Quick elimination: if even the 6 largest Fibonacci numbers up to n do
       not sum to at least n, there is no solution. */
    seq_t sum_largest6 = fib[fibcnt - 1] + fib[fibcnt - 2]
                       + fib[fibcnt - 3] + fib[fibcnt - 4]
                       + fib[fibcnt - 5] + fib[fibcnt - 6];
    if (sum_largest6 < n)
        return 0;

    seq_t count = 0;
    for (idx_t i = 0; i + 5 < fibcnt; ++i) {
        seq_t s1 = fib[i];
        for (idx_t j = i + 1; j + 4 < fibcnt; ++j) {
            seq_t s2 = s1 + fib[j];
            for (idx_t k = j + 1; k + 3 < fibcnt; ++k) {
                seq_t s3 = s2 + fib[k];
                for (idx_t l = k + 1; l + 2 < fibcnt; ++l) {
                    seq_t s4 = s3 + fib[l];
                    for (idx_t m = l + 1; m + 1 < fibcnt; ++m) {
                        seq_t s5 = s4 + fib[m];
                        for (idx_t o = m + 1; o < fibcnt; ++o) {
                            seq_t total = s5 + fib[o];
                            if (total == n)
                                ++count;
                        }
                    }
                }
            }
        }
    }
    return count;
}
