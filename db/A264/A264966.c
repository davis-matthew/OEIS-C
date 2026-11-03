/*
 * A264966  Permutation of nonnegative integers: a(n) = A057889(A263273(n)).
 * https://oeis.org/A264966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A057889(A263273(n))
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9842 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) as the composition of an adjusted reverse in base 3
   followed by an adjusted reverse in base 2. */

#include <stdbool.h>

static inline seq_t adjusted_reverse(seq_t n, unsigned base)
{
    if (n == 0)
        return (seq_t)0;

    /* Count trailing zeros in the given base. */
    idx_t tz = 0;
    seq_t tmp = n;
    while (tmp % base == 0) {
        tmp /= base;
        tz++;
    }

    /* Reverse the remaining digits. */
    seq_t rev = 0;
    while (tmp > 0) {
        rev = rev * base + (tmp % base);
        tmp /= base;
    }

    /* Multiply by base^tz to re‑append the trailing zeros. */
    seq_t powB = 1;
    for (idx_t i = 0; i < tz; ++i)
        powB *= base;

    return rev * powB;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = adjusted_reverse(x, 3); /* A263273: base‑3 adjusted reverse */
    x = adjusted_reverse(x, 2); /* A057889: base‑2 adjusted reverse */
    return x;
}
