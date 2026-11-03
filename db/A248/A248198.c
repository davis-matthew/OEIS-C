/*
 * A248198  a(n) = ceiling(n^3*(Pi/2)).
 * https://oeis.org/A248198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^3 * PI / 2)
 * Range:     u8 none, u16 n <= 1, u32 none, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a pre‑computed string of PI digits. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static const char *pi_digits = 
"31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253521170679805645257491928"
"88466440582353781659612800686858506700606105967079353063517234135136248204568800073046281887324128685";

static seq_t pi_scaled_of(idx_t suffix_len)
{
    seq_t res = 0;
    /* suffix_len is number of decimal digits k; we need k+1
       to obtain floor(PI * 10^k).  */
    size_t need = (size_t)suffix_len + 1;
    for (size_t i = 0; i < need; ++i)
    {
        res = mul_small(res, 10) + (pi_digits[i] - '0');
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* not used in OEIS, but guard */
        return 0;

    /* Determine number of digits in n */
    size_t d = (size_t)num_digits_idx(n);

    /* Choose k large enough so that 10^k > n^3 / 2.          */
    /* For n up to 10^5 (d ≤ 5), k = 3*d + 7 is far more than enough. */
    idx_t k = 3 * (idx_t)d + 7;

    /* pi_scaled = floor(PI * 10^k) */
    seq_t pi_scaled = pi_scaled_of(k);

    /* n^3 */
    seq_t nn = (seq_t)n;
    seq_t n3 = nn * nn * nn;

    /* denom = 2 * 10^k */
    seq_t pow10k = pow10(k);
    seq_t denom = pow10k * 2;

    /* Multiply and divide */
    seq_t product = n3 * pi_scaled;
    seq_t floor_val = product / denom;
    seq_t rem = product - floor_val * denom;

    return rem == 0 ? floor_val : floor_val + 1;
}
