/*
 * A244118  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of 1 as Sum_{k=0..n} T(n,k)*binomial(n,k).
 * https://oeis.org/A244118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r and k satisfy * n = r*(r+1)/2 + k (0 ≤ k ≤
 *            r), * with T(r,0)=0^r (1 for r=0, 0 otherwise) * and for k>0,
 *            T(r,k) = (1+k)^(k-1) * (-k)^(r-k)
 * Range:     i8 n <= 17, i16 n <= 33, i32 n <= 64, i64 n <= 167,
 *            i128 n <= 431, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find row r such that cumulative lengths up to row r-1 ≤ n < cumulative up to row r.
       cumulative length up to row r-1 = r*(r+1)/2 */
    idx_t curr = 0;
    idx_t r = 0;
    while (curr + r + 1 <= n) {
        curr += r + 1;
        ++r;
    }
    idx_t k = n - curr;            /* column index within row r */
    /* Handle k == 0 */
    if (k == 0) {
        if (r == 0) return (seq_t)1; /* 0^0 = 1 */
        return (seq_t)0;             /* 0^r = 0 for r > 0 */
    }
    /* Compute (1+k)^(k-1) */
    seq_t p1 = big_pow((seq_t)(1 + k), (seq_t)(k - 1));
    /* Compute k^(r-k) */
    seq_t p2 = big_pow((seq_t)(k), (seq_t)(r - k));
    /* Combine magnitude */
    seq_t term = p1 * p2;
    /* Apply sign from (-k)^(r-k) */
    if ( ((r - k) & 1) != 0 ) {
        term = -term;
    }
    return term;
}
