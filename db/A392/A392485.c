/*
 * A392485  Number of n X n Boolean matrices that are row-consistent but not
 *          column-consistent.
 * https://oeis.org/A392485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^{k+1} * binomial(n,k) * (2^{n-k} - 1)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   binomial, ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pos_sum = 0;   /* sum of terms with positive sign */
    seq_t neg_sum = 0;   /* sum of terms with negative sign */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin   = binomial(n, k);                     /* C(n,k) */
        seq_t pow2_val = pow2(n - k);                     /* 2^{n-k}   */
        seq_t base = pow2_val - 1;                        /* 2^{n-k} - 1   */
        seq_t pow_base = ipow_i32(base, n);                /* (2^{n-k}-1)^n */
        seq_t term = bin * pow_base;                      /* contribution */

        if ((k & 1) == 1)   /* k odd => (-1)^{k+1}=+1 */
            pos_sum += term;
        else                /* k even => (-1)^{k+1}=-1 */
            neg_sum += term;
    }

    return pos_sum - neg_sum;  /* pos_sum ≥ neg_sum for all n ≥ 1 */
}
