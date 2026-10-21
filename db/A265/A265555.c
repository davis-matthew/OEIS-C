/*
 * A265555  a(n) = smallest base-8 palindrome m >= n such that every base-8
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 8.
 * https://oeis.org/A265555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: minimal base‑8 palindrome >= n with
   digit‑wise dominance.  The result is returned as the decimal
   integer whose decimal digits are the base‑8 digits of that
   palindrome (e.g. base‑8 “11” → 11, base‑8 “22” → 22). */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Step 1: extract base‑8 digits of n (least significant first). */
    size_t len = 0;
    uint8_t d[64];
    idx_t t = n;
    if (t == 0) {
        d[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            d[len++] = t & 7U;   /* t % 8, shift is fine for unsigned */
            t >>= 3U;            /* equivalent to t /= 8 */
        }
    }

    /* Step 2: construct the minimal palindrome that dominates n.
       For each mirrored pair (i, j) we set both digits to
       max(d[i], d[j]). */
    uint8_t m[64];
    for (size_t i = 0; i < len; ++i) {
        size_t j = len - 1 - i;
        uint8_t val = d[i] > d[j] ? d[i] : d[j];
        m[i] = val;
        m[j] = val;
    }

    /* Step 3: convert the palindrome's base‑8 digits to a decimal
       integer that has those digits in decimal. */
    seq_t res = 0;
    for (size_t k = 0; k < len; ++k) {
        /* build from most significant digit (m[len-1]) downwards. */
        res = res * 10 + (seq_t)m[len - 1 - k];
    }

    return res;
}
