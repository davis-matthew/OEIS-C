/*
 * A359538  Number of partitions of n into at most 3 distinct positive
 *          Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A359538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most 3 distinct
 *            positive Fibonacci numbers (1 counted only once)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the base case a(0) = 1 */
    if (n == 0) return 1;

    /* Build the list of distinct Fibonacci numbers <= n.
       The sequence starts 1, 2, 3, 5, 8, ... (1 appears once). */
    seq_t fibs[94];          /* Fibonacci numbers up to 2^64 fit in 93 numbers */
    idx_t  m = 0;            /* actual number of fibs in the array */
    seq_t  sn = (seq_t)n;    /* cast n to seq_t for safe arithmetic */

    if (sn >= 1) fibs[m++] = 1;
    if (sn >= 2) fibs[m++] = 2;

    seq_t a1 = 1, b1 = 2;    /* previous two Fibonacci numbers */
    for (;;) {
        seq_t c = a1 + b1;
        if (c > sn) break;
        fibs[m++] = c;
        a1 = b1;
        b1 = c;
    }

    seq_t count = 0;

    /* Count subsets of size 1 */
    for (idx_t i = 0; i < m; ++i) {
        if (fibs[i] == sn) ++count;
    }

    /* Count subsets of size 2 */
    for (idx_t i = 0; i < m; ++i) {
        for (idx_t j = i + 1; j < m; ++j) {
            if (fibs[i] + fibs[j] == sn) ++count;
        }
    }

    /* Count subsets of size 3 */
    for (idx_t i = 0; i < m; ++i) {
        for (idx_t j = i + 1; j < m; ++j) {
            for (idx_t k = j + 1; k < m; ++k) {
                if (fibs[i] + fibs[j] + fibs[k] == sn) ++count;
            }
        }
    }

    return count;
}
