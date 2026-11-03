/*
 * A380585  a(n) = floor(n^2 / 10^m) + (n^2 mod 10^m) where m is the number of
 *          decimal digits in n.
 * https://oeis.org/A380585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = floor(n^2 / 10^m) + (n^2 mod 10^m), where m =
 *            digit_len(n)
 * Range:     u8 n <= 15, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9343 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

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
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;                     /* n^2 */
    int m   = num_digits(nn);                /* number of decimal digits in n */
    seq_t p   = pow10((idx_t)m);            /* 10^m */
    seq_t high = sq / p;                    /* floor(n^2 / 10^m) */
    seq_t low  = sq % p;                    /* n^2 mod 10^m */
    return high + low;
}
