/*
 * A280817  a(n) = A049501(sigma(n)).
 * https://oeis.org/A280817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049501(A000203(n))
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 125 terms through n = 9336 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Compute the major index of an integer m:
 * sum of positions i (1‑based from the most significant bit)
 * where bit i = 1 and bit i+1 = 0 in the binary representation of m. */
static seq_t major_index(seq_t m)
{
    /* 0 or 1 have no binary descents */
    if (m < 2) return 0;

    /* Determine the length L of the binary representation of m. */
    idx_t L = 0;
    seq_t t = m;
    while (t != 0) {
        t >>= 1;
        L++;
    }

    /* Scan bits from most significant to the second least significant. */
    seq_t sum = 0;
    for (idx_t pos = 1; pos < L; pos++) {
        seq_t bit1 = (m >> (L - pos)) & 1;
        seq_t bit2 = (m >> (L - pos - 1)) & 1;
        if (bit1 && !bit2) {
            sum += (seq_t)pos;
        }
    }

    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);   /* A000203: sum of divisors of n */
    return major_index(sigma);
}
