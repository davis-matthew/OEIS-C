/*
 * A265511  a(n) = largest base-3 palindrome m <= n such that every base-3
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 10.
 * https://oeis.org/A265511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = largest base‑3 palindrome m ≤ n such that each base‑3 *
 *            digit of m is ≤ the corresponding digit of n; result is returned
 *            in * decimal (base‑10)
 * Range:     u8 n <= 279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_DIGITS 65  /* 3^41 > 2^64, so 65 digits suffice for uint64_t */

seq_t a(idx_t n)
{
    /* Special case n = 0 – the only palindrome is 0. */
    if (n == 0)
        return (seq_t)0;

    /* Convert n to base‑3 digits (least significant first). */
    unsigned char d[MAX_DIGITS];
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        d[len++] = (unsigned char)(tmp % 3);
        tmp /= 3;
    }

    /* Precompute powers of 3 up to len‑1 in seq_t. */
    seq_t pow3[MAX_DIGITS];
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i < len; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* Find the longest palindrome with the digit‑wise constraint. */
    for (idx_t L = len; L >= 1; --L) {
        if (L == 1) {
            /* Single‑digit palindrome – always valid, even if 0. */
            return (seq_t)d[0];
        }

        /* The most significant digit (position L‑1) must be > 0. */
        unsigned char msb = d[0] < d[L - 1] ? d[0] : d[L - 1];
        if (msb == 0)
            continue;  /* No palindrome of this length. */

        /* Build the palindrome: digit[i] = min(d[i], d[L-1-i]) for all i. */
        seq_t val = 0;
        for (idx_t i = 0; i < L; ++i) {
            unsigned char left  = d[i];
            unsigned char right = d[L - 1 - i];
            unsigned char digit = left < right ? left : right;
            val += pow3[i] * (seq_t)digit;
        }
        return val;
    }

    /* Fallback – should never happen. */
    return (seq_t)0;
}
