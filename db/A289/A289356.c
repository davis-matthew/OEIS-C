/*
 * A289356  Least number k such that n^2 + n + k is prime.
 * https://oeis.org/A289356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k ≥1 | n^2 + n + k is prime}
 * Range:     u8 n <= 15, u16 n <= 559, u32 n <= 85, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t next_prime_seq(seq_t start)
{
    /* Return the smallest prime ≥ start. */
    if (start <= 2) return 2;
    seq_t p = start;
    if ((p & 1) == 0) p++;          /* make odd if even */
    for (;;) {
        if (is_prime_big(p)) return p;
        p += 2;                     /* test only odd numbers */
    }
}

seq_t a(idx_t n)
{
    /* Compute m = n^2 + n and find the next prime ≥ m + 1. */
    seq_t m     = (seq_t)n * ((seq_t)n + 1);   /* n^2 + n */
    seq_t start = m + 1;
    seq_t p     = next_prime_seq(start);
    return p - m;                               /* minimal k */
}
