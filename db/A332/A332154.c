/*
 * A332154  a(n) = 5*(10^(2*n+1)-1)/9 - 10^n.
 * https://oeis.org/A332154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 - 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Fast computation using the closed‑form expression */
    idx_t exp  = 2*n + 1;         /* 2*n+1, safe because n ≤ 10 000 */
    seq_t pow_big = pow10(exp);   /* 10^(2*n+1) */
    seq_t pow_n   = pow10(n);     /* 10^n */

    seq_t tmp      = pow_big - 1;   /* (10^(2*n+1) - 1) */
    seq_t divided  = tmp / 9;       /* (10^(2*n+1) - 1) / 9 */
    seq_t multiplied = mul_small(divided, 5); /* 5 * ((10^(2*n+1)-1)/9) */

    return multiplied - pow_n;          /* final result */
}
