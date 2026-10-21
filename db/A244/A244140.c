/*
 * A244140  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n*(-1)^n as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th element of the triangle T(n,k) where T(n,0)=0,
 *            T(1,1)=-1, and for k>1, T(n,k)=(-1)^k * k * (k-2)^(n-2)
 * Range:     i8 n <= 19, i16 n <= 42, i32 n <= 76, i64 n <= 185,
 *            i128 n <= 433, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find the row r and column c corresponding to index n. */
    idx_t r = 0;
    idx_t cum = 1; /* number of entries up to and including row r */
    while (cum <= n) {
        r++;
        cum += r + 1; /* next row has r+1 elements */
    }
    idx_t before = cum - (r + 1);      /* entries before row r */
    idx_t c = n - before;              /* column within the row, 0 ≤ c ≤ r */

    /* Handle special cases for k = 0 and k = 1. */
    if (c == 0) {
        return (seq_t)0;
    }
    if (c == 1) {
        return (r == 1) ? (seq_t)(-1) : (seq_t)0;
    }

    /* General case: k > 1. */
    seq_t base = (seq_t)(c - 2);
    seq_t exp  = (seq_t)(r - 2);  /* r ≥ 2 here, so exp ≥ 0 */
    seq_t pow_val = big_pow(base, exp);   /* (c-2)^(r-2) */

    seq_t c_seq = (seq_t)c;
    seq_t sign  = (c & 1) ? -(seq_t)1 : (seq_t)1;  /* (-1)^c */

    return pow_val * c_seq * sign;
}
