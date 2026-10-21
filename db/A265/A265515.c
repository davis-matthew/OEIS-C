/*
 * A265515  a(n) = largest base-5 palindrome m <= n such that every base-5
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 10.
 * https://oeis.org/A265515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A265515: largest base‑5 palindrome m ≤ n such that each base‑5
 * digit of m is ≤ the corresponding digit of n (aligned on the right). */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 specially. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Store the base‑5 digits of n, least significant first. */
    uint8_t d[64];          /* enough for any 64‑bit integer */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > (idx_t)0) {
        d[len++] = (uint8_t)(tmp % (idx_t)5);
        tmp /= (idx_t)5;
    }

    seq_t best = (seq_t)0;

    /* Try all palindrome lengths from 1 up to len. */
    for (size_t l = (size_t)1; l <= len; ++l) {
        /* Build the palindrome digits (LSB first). */
        uint8_t p[64] = {0};
        size_t i, j, mid = (l + (size_t)1) / (size_t)2;
        for (i = (size_t)0; i < mid; ++i) {
            j = l - (size_t)1 - i;
            uint8_t bound = d[i] < d[j] ? d[i] : d[j];
            p[i] = p[j] = bound;
        }

        /* Leading digit (most significant) must be non‑zero unless l==1. */
        if (l > (size_t)1 && p[l - (size_t)1] == (uint8_t)0)
            continue;

        /* Convert the digits to a value in base‑10. */
        seq_t val = (seq_t)0;
        for (j = l; j > (size_t)0; --j) {
            val = val * (seq_t)5 + (seq_t)p[j - (size_t)1];
        }

        if (val > best)
            best = val;
    }

    return best;
}
