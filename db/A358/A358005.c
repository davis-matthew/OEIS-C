/*
 * A358005  Number of partitions of n into 5 distinct positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A358005
 *
 * Offset:    19    (a(19) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1051, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute-force enumeration of 5 distinct Fibonacci numbers <= n */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Generate distinct Fibonacci numbers up to N (1, 2, 3, 5, …). */
    seq_t fib[64];
    size_t m = 0;
    fib[m++] = 1;
    if (m < sizeof(fib)/sizeof(fib[0])) fib[m++] = 2;
    while (1) {
        seq_t next = fib[m-2] + fib[m-1];
        if (next > N) break;
        if (m >= sizeof(fib)/sizeof(fib[0])) break;
        fib[m++] = next;
    }

    if (m < 5) return 0;        /* not enough Fibonacci numbers to form a 5-partition */

    seq_t count = 0;
    for (size_t i = 0; i + 4 < m; ++i) {
        for (size_t j = i + 1; j + 3 < m; ++j) {
            for (size_t k = j + 1; k + 2 < m; ++k) {
                for (size_t l = k + 1; l + 1 < m; ++l) {
                    for (size_t r = l + 1; r < m; ++r) {
                        seq_t sum = fib[i] + fib[j] + fib[k] + fib[l] + fib[r];
                        if (sum == N) ++count;
                    }
                }
            }
        }
    }

    return count;
}
