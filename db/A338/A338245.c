/*
 * A338245  Nonnegative values in A117966, in order of appearance.
 * https://oeis.org/A338245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(n) = A117966(A132141(n)) for n > 0, where A132141(n)
 *            = n + (3^floor(log_3(2n)) - 1)/2 and A117966 reads ternary
 *            digits with 2 -> -1
 * Range:     u8 n <= 133, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Validated: standard, pass, 84 terms through n = 9326 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A132141_a(idx_t n)
{
    seq_t target = (seq_t)n * 2;
    seq_t p = 1;
    while (p <= target / 3) {
        p *= 3;
    }
    return (seq_t)n + (p - 1) / 2;
}

/* m has leading ternary digit 1, so every prefix value is positive (no negatives arise) */
static seq_t A117966_rec(seq_t m)
{
    if (m == 1) return (seq_t)1;
    seq_t base = A117966_rec(m / 3);
    int d = (int)(m % 3);
    if (d == 0) return base * (seq_t)3;
    if (d == 1) return base * (seq_t)3 + (seq_t)1;
    return base * (seq_t)3 - (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t m = A132141_a(n);
    return A117966_rec(m);
}
