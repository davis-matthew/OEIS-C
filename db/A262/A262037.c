/*
 * A262037  Replace the second half of digits of n with the first half in
 *          reverse order.
 * https://oeis.org/A262037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = replace the second half of the decimal digits of n with
 *            the first half reversed
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 8758 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n == 0 yields 0 directly. */
    if (n == 0)
        return 0;

    /* Count decimal digits of n. */
    idx_t len = 0;
    idx_t t = n;
    while (t > 0) {
        len++;
        t /= 10;
    }

    /* Number of digits in each half. */
    idx_t half = len / 2;                     /* floor(len/2) */

    /* 10^(len - half) isolates the first half. */
    seq_t pow10_rest = pow10(len - half);
    seq_t firstHalf = (seq_t)n / pow10_rest;   /* most significant half */

    /* Middle digit (unchanged) if len is odd. */
    seq_t middle = 0;
    if (len & 1U) {
        seq_t pow10_mid = pow10(len - half - 1);
        middle = ((seq_t)n / pow10_mid) % 10;
    }

    /* Reverse the first half's decimal digits. */
    seq_t rev = 0;
    seq_t temp = firstHalf;
    for (idx_t i = 0; i < half; ++i) {
        rev = rev * 10 + (temp % 10);
        temp /= 10;
    }

    seq_t pow10_half = pow10(half);

    if (len & 1U) { /* odd length */
        return firstHalf * pow10(half + 1) + middle * pow10_half + rev;
    } else {        /* even length */
        return firstHalf * pow10_half + rev;
    }
}
