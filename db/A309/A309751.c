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
 * Helpers:   inv_mod_2_pow, make_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/inv_mod_2_pow.h"
#include "../utils/make_mask.h"

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
