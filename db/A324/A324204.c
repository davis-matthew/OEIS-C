/*
 * A324204  a(n) is the smallest denominator D of the fraction N/D with 1 <= D
 *          < 2^n which is closest to (3/2)^n.
 * https://oeis.org/A324204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = min(inv, 2^n - inv) where inv is the modular inverse of
 *            3^n modulo 2^n. A fast O(n) algorithm is used: compute 3^n mod
 *            2^n by binary exponentiation, then compute the inverse by Hensel
 *            lifting. The result is the smallest denominator D < 2^n that
 *            minimizes |N - D*(3/2)^n|, and ties are broke...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* 3^n mod 2^n */
    seq_t mod  = ((seq_t)1 << n);
    seq_t mask = mod - (seq_t)1;

    seq_t res   = (seq_t)1;
    seq_t base  = (seq_t)3;
    idx_t exp   = n;
    while (exp) {
        if (exp & 1)
            res = (res * base) & mask;
        base = (base * base) & mask;
        exp >>= 1;
    }
    seq_t p = res;   /* p = 3^n mod 2^n */

    /* Inverse of p modulo 2^n using Hensel lifting */
    seq_t inv      = (seq_t)1;   /* inverse modulo 2 */
    seq_t cur_mask = (seq_t)1;   /* 2^1-1 */
    for (idx_t k = 1; k < n; ++k) {
        cur_mask = (cur_mask << 1) | (seq_t)1;          /* next mask 2^(k+1)-1 */
        seq_t temp = (p & cur_mask) * inv & cur_mask;   /* (p * inv) mod 2^(k+1) */
        seq_t diff = ((seq_t)2 - temp) & cur_mask;     /* 2 - temp mod 2^(k+1) */
        inv = (inv * diff) & cur_mask;                /* update inverse */
    }

    seq_t d1 = inv;
    seq_t d2 = mod - inv;      /* 2^n - inv */

    return (d1 < d2) ? d1 : d2;
}
