/*
 * A359511  Number of partitions of n into at most 2 positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A359511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n into at most two positive
 *            Fibonacci numbers (the Fibonacci 1 is counted as a single type)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Zero-length partitions */
    if (n == 0)
        return (seq_t)1;

    /* Generate all Fibonacci numbers <= n.
       The sequence used is 1,2,3,5,8,... (1 counted only once). */
    seq_t fibs[64];          /* holds at most 64 Fibonacci numbers; 10 000 needs < 20 */
    size_t fib_cnt = 0;

    fibs[fib_cnt++] = (seq_t)1;                     /* first Fibonacci number */

    if (n >= 2) {
        fibs[fib_cnt++] = (seq_t)2;                 /* second distinct Fibonacci number */
        seq_t prev = (seq_t)1;
        seq_t curr = (seq_t)2;

        while (1) {
            seq_t next = prev + curr;
            if (next > (seq_t)n)
                break;
            fibs[fib_cnt++] = next;
            prev = curr;
            curr = next;
        }
    }

    seq_t count = 0;

    /* Count the single-part partition if n itself is Fibonacci. */
    for (size_t i = 0; i < fib_cnt; ++i) {
        if (fibs[i] == (seq_t)n) {
            ++count;
            break;
        }
    }

    /* Count unordered pairs (f,g) of Fibonacci numbers with f+g=n.
       Iterate only over f <= n/2 to avoid double counting. */
    idx_t half = n >> 1;
    for (size_t i = 0; i < fib_cnt; ++i) {
        seq_t f = fibs[i];
        if (f > (seq_t)half)
            break;

        seq_t other = (seq_t)n - f;
        /* Linear search for 'other' in the Fibonacci list. */
        for (size_t j = 0; j < fib_cnt; ++j) {
            if (fibs[j] == other) {
                ++count;
                break;
            }
        }
    }

    return count;
}
