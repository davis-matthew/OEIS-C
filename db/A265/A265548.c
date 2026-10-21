/*
 * A265548  a(n) = smallest base-5 palindrome m >= n such that every base-5
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 10.
 * https://oeis.org/A265548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑5 palindrome m ≥ n such that every base‑5
 *            digit of n is ≤ the corresponding digit of m
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Convert n to base‑5 digits (least‑significant first) and
   return the smallest palindrome satisfying the digitwise
   inequality above. */
seq_t a(idx_t n)
{
    /* Step 1: Produce base‑5 digits of n. */
    uint8_t ndigs[64];
    idx_t len = 0;
    seq_t val = (seq_t)n;
    while (val != 0) {
        ndigs[len++] = (uint8_t)(val % 5);
        val = val / 5;
    }

    /* Length 0 corresponds to n = 0 → 0 is already a palindrome. */
    if (len == 0)
        return 0;

    /* Step 2: Compute the minimal left‑half digits of the palindrome. */
    idx_t half = (len + 1) / 2;          /* number of left positions */
    uint8_t left[64];

    for (idx_t i = 0; i < half; ++i) {
        uint8_t req = ndigs[i];
        idx_t mir = len - 1 - i;        /* mirrored position of i */
        if (mir < len && ndigs[mir] > req)
            req = ndigs[mir];
        /* The leading digit (i == 0) must be at least 1. */
        if (i == 0 && req < 1)
            req = 1;
        left[i] = req;
    }

    /* Step 3: Reconstruct the full palindrome from the left half. */
    seq_t m = 0;
    for (idx_t pos = len; pos > 0; --pos) {
        idx_t i = pos - 1;               /* current position (0 = least) */
        uint8_t d = (i < half) ? left[i] : left[len - 1 - i];
        m = m * 5 + d;
    }
    return m;
}
