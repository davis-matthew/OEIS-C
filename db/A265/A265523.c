/*
 * A265523  a(n) = largest base-9 palindrome m <= n such that every base-9
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 10.
 * https://oeis.org/A265523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = the largest base‑9 palindrome m ≤ n such that every
 * base‑9 digit of m is ≤ the corresponding digit of n (from the
 * least significant digit).  m is returned in base‑10. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* n = 0 → only palindrome is 0 */
    if (n == 0)
        return 0;

    /* Convert n to base‑9 digits, least‑significant first. */
    uint8_t d[65];          /* enough for any 64‑bit integer */
    size_t L = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        d[L++] = (uint8_t)(tmp % 9);
        tmp /= 9;
    }

    /* Try palindrome lengths from full length down to 1. */
    for (size_t len = L; len >= 1; --len) {
        uint8_t p[65];

        /* Build a palindrome of this length:
           p[i] = min(d[i], d[len-1-i]) for all 0 ≤ i < len. */
        for (size_t i = 0; i < len; ++i) {
            size_t j = len - 1 - i;
            uint8_t x = d[i];
            if (x > d[j]) x = d[j];
            p[i] = p[j] = x;
        }

        /* Discard leading‑zero palindromes except the single‑digit 0. */
        if (len > 1 && p[len - 1] == 0)
            continue;

        /* Convert the palindrome back to base‑10. */
        seq_t res = 0;
        for (size_t i = len; i > 0; --i) {
            res = res * (seq_t)9 + (seq_t)p[i - 1];
        }
        return res;
    }

    /* Fallback – should never happen. */
    return 0;
}
