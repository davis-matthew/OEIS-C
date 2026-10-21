/*
 * A265524  a(n) = largest base-9 palindrome m <= n such that every base-9
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 9.
 * https://oeis.org/A265524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): largest base‑9 palindrome m ≤ n such that every
   base‑9 digit of m is ≤ the corresponding digit of n.
   The returned value is the base‑9 digit string of m interpreted as a
   base‑10 integer. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* (1) Convert n to base‑9, digits in least‑significant order. */
    uint8_t lsd[64] = {0};
    int len = 0;
    idx_t t = n;
    if (t == 0) {
        lsd[0] = 0;
        len = 1;
    } else {
        while (t) {
            lsd[len++] = t % 9;
            t /= 9;
        }
    }

    /* (2) Reverse to obtain most‑significant order. */
    uint8_t msd[64];
    for (int i = 0; i < len; ++i)
        msd[i] = lsd[len - 1 - i];

    /* (3) Build the largest palindrome respecting the per‑digit limits. */
    uint8_t pal[64];
    for (int i = 0; i < len; ++i) {
        int j = len - 1 - i;
        if (i > j) break;                /* already set */
        uint8_t d;
        if (i == j)                      /* middle digit (odd length) */
            d = msd[i];
        else                            /* mirrored pair */
            d = (msd[i] < msd[j]) ? msd[i] : msd[j];
        pal[i] = pal[j] = d;
    }

    /* (4) Interpret the palindrome digits as a base‑10 integer. */
    seq_t val = 0;
    for (int i = 0; i < len; ++i)
        val = val * 10 + pal[i];

    return val;
}
