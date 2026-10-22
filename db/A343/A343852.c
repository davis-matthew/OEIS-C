/*
 * A343852  a(n) is the least k > 0 such that the binary expansions of k and
 *          of n + k have the same numbers of 0's and of 1's.
 * https://oeis.org/A343852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least k > 0 such that the binary expansions of k and (n + k)
 * have the same number of 1’s and the same number of 0’s.
 * Equivalently: popcount(k) == popcount(n + k) and bitlen(k) == bitlen(n + k). */

#include <stdbool.h>

/* Count the number of set bits in a seq_t value.  Works for any
 * seq_t that supports the usual bitwise operations and comparison with 0. */
static inline int popcount_seq_t(seq_t x)
{
    int cnt = 0;
    for (seq_t y = x; y > 0; y >>= 1)
        cnt += (int)(y & 1);
    return cnt;
}

/* Return the bit length (position of the most significant 1 + 1).
 * Assumes x > 0. */
static inline int bitlen_seq_t(seq_t x)
{
    int len = 0;
    while (x > 0) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast the index to seq_t */
    seq_t k = 1;                 /* k must be > 0 */

    for (;; ++k) {
        seq_t sum = k + N;
        if (popcount_seq_t(k) == popcount_seq_t(sum) &&
            bitlen_seq_t(k)   == bitlen_seq_t(sum))
            return k;
    }
}
