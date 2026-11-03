/*
 * A264965  Permutation of nonnegative integers: a(n) = A263273(A057889(n)).
 * https://oeis.org/A264965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A263273(A057889(n))
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9860 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the adjusted reverse in base 2 followed by the adjusted reverse in base 3. */

#include <stdint.h>

/* adjusted reverse of n in base 2, preserving trailing zeros */
static seq_t adj_rev_base2(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)0;
    seq_t x = n;
    unsigned t = 0;
    while ((x % (seq_t)2) == (seq_t)0) { x /= (seq_t)2; ++t; }
    seq_t rev = (seq_t)0;
    seq_t y = x;
    while (y > (seq_t)0) {
        rev = rev * (seq_t)2 + (y % (seq_t)2);
        y /= (seq_t)2;
    }
    for (unsigned i = 0; i < t; ++i) rev *= (seq_t)2;
    return rev;
}

/* adjusted reverse of n in base 3, preserving trailing zeros */
static seq_t adj_rev_base3(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)0;
    seq_t x = n;
    unsigned t = 0;
    while ((x % (seq_t)3) == (seq_t)0) { x /= (seq_t)3; ++t; }
    seq_t rev = (seq_t)0;
    seq_t y = x;
    while (y > (seq_t)0) {
        rev = rev * (seq_t)3 + (y % (seq_t)3);
        y /= (seq_t)3;
    }
    for (unsigned i = 0; i < t; ++i) rev *= (seq_t)3;
    return rev;
}

/* Sequence function */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t b2 = adj_rev_base2(sn);
    return adj_rev_base3(b2);
}
