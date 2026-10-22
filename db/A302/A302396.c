/*
 * A302396  Number of families of 4-subsets of an n-set that cover every
 *          element.
 * https://oeis.org/A302396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n,k) * 2^binomial(n-k,4)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   binom4_idx, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/binom4_idx.h"

seq_t a(idx_t n) {
    /* handle the trivial case */
    if (n == 0) return (seq_t)1;

    seq_t even = 0;
    seq_t odd  = 0;

    seq_t binom_nk = 1;                 /* binomial(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        idx_t rem = n - k;
        idx_t expo = binom4_idx(rem);   /* C(n-k,4) as idx_t */

        seq_t pow_val = pow2(expo);     /* 2^C(n-k,4) */

        seq_t term = binom_nk * pow_val;/* binomial(n,k) * pow2 */

        if (k & 1)
            odd += term;
        else
            even += term;

        /* update binomial(n,k+1) for next iteration */
        if (k < n) {
            binom_nk = binom_nk * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    /* a(n) is non‑negative; in all cases even >= odd */
    if (even >= odd)
        return even - odd;
    return 0;   /* safety: shouldn't occur */
}
