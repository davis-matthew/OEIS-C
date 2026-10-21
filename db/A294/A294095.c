/*
 * A294095  Sum of the differences of the larger and smaller parts in the
 *          partitions of n into two parts with the larger part squarefree and
 *          smaller part prime.
 * https://oeis.org/A294095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n - 2i) * A010051(i) * A008966(n -
 *            i)
 * Range:     u8 n <= 59, u16 n <= 1169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree_big.h"

/* Test whether m is a prime number (trial division). */
static inline bool is_prime_small(idx_t m) {
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* iterate over possible smaller part i (prime) */
    for (idx_t i = 2; i <= n / 2; ++i) {
        if (!is_prime_small(i))
            continue;
        idx_t big = n - i;            /* larger part */
        if (!is_squarefree_big(big))
            continue;
        seq_t term = (seq_t)(n - 2 * i);
        sum += term;
    }
    return sum;
}
