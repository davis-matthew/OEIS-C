/*
 * A332154  a(n) = 5*(10^(2*n+1)-1)/9 - 10^n.
 * https://oeis.org/A332154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 - 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
