/*
 * A309631  a(n) is the smallest positive integer divisible by n such that it
 *          is possible to strike out a digit from its decimal expansion
 *          (apart from trailing zeros) so that the resulting number is
 *          nonzero and divisible by n.
 * https://oeis.org/A309631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 108, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest positive integer m divisible by n such that
   by deleting a single non‑trailing‑zero digit from m (in base 10)
   we obtain a non‑zero multiple of n. */

#include <stdbool.h>
#include <stdint.h>

/* Determine the length in decimal digits of a positive value x. */
static inline int num_digits(seq_t x)
{
    int d = 0;
    while (x > 0) {
        d++;
        x /= 10;
    }
    return d;
}

/* Return true iff m satisfies the digit‑striking requirement for n. */
static bool has_property(seq_t m, seq_t n)
{
    /* m is positive and divisible by n. */
    int len = num_digits(m);
    if (len == 0) return false;

    /* Store digits from least significant to most.  The array size is
       hard‑coded to accommodate any value that fits in seq_t (max 39
       for 128‑bit). */
    uint8_t digits[128];
    seq_t tmp = m;
    for (int i = 0; i < len; ++i) {
        digits[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Compute the number of trailing zeros.  Digits 0 .. tz-1 are zero
       and may not be removed. */
    int tz = 0;
    while (tz < len && digits[tz] == 0) tz++;

    /* Pre‑compute powers of ten up to len. */
    seq_t pow10[128];
    pow10[0] = 1;
    for (int i = 1; i <= len; ++i) pow10[i] = pow10[i - 1] * 10;

    /* Try removing each non‑trailing digit. */
    for (int pos = tz; pos < len; ++pos) {
        seq_t low  = m % pow10[pos];
        seq_t high = (pos + 1 <= len) ? (m / pow10[pos + 1]) : 0;
        seq_t mprime = high * pow10[pos] + low;
        if (mprime != 0 && mprime % n == 0) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        seq_t m = sn * k;
        if (has_property(m, sn))
            return m;
    }
}
