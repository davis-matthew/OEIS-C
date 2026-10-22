/*
 * A338616  a(n) is twice the number of parts in all partitions of n into
 *          consecutive parts.
 * https://oeis.org/A338616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ k where the sum runs over all k ≥ 1 such that n is
 *            representable as the sum of k consecutive positive integers.
 *            This is equivalent to summing the lengths of all representations
 *            of n as a sum of consecutive positive integers
 * Range:     u8 n <= 493, u16 n <= 8118, u32 n <= 8118, u64 n <= 8118,
 *            u128 n <= 8118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t etc. but seq_t/idx_t are provided */

/* Computes a(n) for n ≥ 1.
   Needs no external tables and runs in O(sqrt(n)). */
seq_t a(idx_t n)
{
    if (n == 0)            /* safety: offset is 1, return 0 for n=0 */
        return 0;

    seq_t sum = 0;                     /* accumulate total parts */
    seq_t two_n = (seq_t)2 * (seq_t)n; /* 2*n fits in seq_t for small n */

    for (idx_t k = 1; ; ++k) {
        /* t = 2*n / k  (floor division) */
        seq_t t = two_n / (seq_t)k;

        /* If the starting integer would be ≤0, no further k will work. */
        if (t <= (seq_t)(k - 1))
            break;

        /* n must be divisible by k when k is odd,
           or n % k == k/2 when k is even.
           The general test is that two_n is divisible by k
           and (t - k + 1) is even. */
        if (two_n % (seq_t)k != 0)
            continue;

        seq_t rem = t - (seq_t)k + 1;
        if ((rem & 1) != 0)          /* rem must be even for integer start */
            continue;

        sum += (seq_t)k;              /* a representation of length k contributes k */
    }

    return (seq_t)2 * sum;           /* each part counted twice */
}
