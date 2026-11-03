/*
 * A309751  Approximation of the 2-adic integer arctan(2) up to 2^n.
 * https://oeis.org/A309751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{i=0..floor(n/2)-1} (-1)^i*2^(2*i+1)/(2*i+1)) mod
 *            2^n
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t make_mask(idx_t n)
{
    /* bit width of the type; works for built-in integer types */
    const idx_t bits = sizeof(seq_t) * 8;
    if (n >= bits) {
        /* For built-in types, modulo 2^n is effectively modulo 2^bits */
        return ~((seq_t)0);
    } else {
        return ((seq_t)1 << n) - 1;
    }
}

static seq_t inv_mod_2_pow(seq_t d, idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t mask = make_mask(n);
    seq_t inv = (seq_t)1;           /* inverse modulo 2^1 */
    idx_t bits = 1;
    while (bits < n) {
        seq_t t = (d * inv) & mask;          /* d * inv  (mod 2^n) */
        seq_t y = ((seq_t)2 - t) & mask;     /* 2 - t    (mod 2^n) */
        inv = (inv * y) & mask;              /* extend to larger modulus */
        bits <<= 1;                          /* next power of two */
    }
    return inv;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    const idx_t m = n >> 1;          /* floor(n/2) */
    if (m == 0) return (seq_t)0;

    seq_t mask_n = make_mask(n);

    seq_t sum = (seq_t)0;
    seq_t pow2 = (seq_t)2;           /* 2^(1) */

    for (idx_t i = 0; i < m; ++i) {
        seq_t denom = (seq_t)(2 * i + 1);      /* odd denominator */

        seq_t inv = inv_mod_2_pow(denom, n);   /* inverse modulo 2^n */

        seq_t term = (pow2 * inv) & mask_n;    /* (-1)^i * 2^(2*i+1)/(2*i+1)  mod 2^n */

        if ((i & 1) == 0) {                     /* even i: add */
            sum = (sum + term) & mask_n;
        } else {                                /* odd i: subtract */
            sum = (sum + mask_n - term + 1) & mask_n;
        }

        pow2 = (pow2 * ((seq_t)4)) & mask_n;  /* next power of 4 */
    }

    return sum;
}
