/*
 * A352802  Expansion of Sum_{k>=0} x^k * Product_{j=0..k-1} (j + 3 * x).
 * https://oeis.org/A352802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 3^k * |Stirling1(n-k,k)|
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code computes the sequence without tabulating all Stirling numbers.  */
/* It uses the recurrence for the product 3^k |Stirling1(m,k)|, which     */
/* requires only small‑integer multiplications.                            */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mul_small.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* Working types */
/*  idx_t is uint64_t; seq_t is the build's integer type  */
    size_t N = (size_t)n;          /* safe upper bound for array sizes      */

    /* Allocate necessary arrays.  All indices below N + 1 are valid. */
    seq_t *cur  = seq_realloc(NULL, 0, N + 1);   /* current row  */
    seq_t *prev = seq_realloc(NULL, 0, N + 1);   /* previous row */
    seq_t *sum  = seq_realloc(NULL, 0, N + 1);   /* accumulator for a[0..N] */

    /* initialise accumulator to zero */
    for (size_t i = 0; i <= N; ++i) sum[i] = (seq_t)0;

    /* ---- row 0 ---------------------------------------------------- */
    /* |S1(0,0)| = 1,  3^0 = 1  ->  b_0 = 1 */
    prev[0] = (seq_t)1;
    sum[0]  = sum[0] + prev[0];          /* a(0) = 1 */
    size_t prev_len = 1;                  /* length of prev row (m = 0) */

    /* ---- compute rows 1 .. N ------------------------------------- */
    for (size_t m = 1; m <= N; ++m) {
        /* current row length is m+1 */
        cur[0] = (seq_t)0;               /* b_0 = 0 for m > 0 */
        for (size_t k = 1; k <= m; ++k) {
            /* term1 = 3 * prev[k-1] */
            seq_t term1 = mul_small(prev[k - 1], 3);
            /* term2 = (m-1) * prev[k]  (only if k < prev_len) */
            seq_t term2 = (k < prev_len) ? mul_small(prev[k], (unsigned int)(m - 1))
                                         : (seq_t)0;
            cur[k] = term1 + term2;
        }

        /* add contributions to a[ ]:  for this row m,
           a[m + k] += cur[k]  (0 <= k <= m)                    */
        for (size_t k = 0; k <= m; ++k) {
            size_t idx = m + k;                /* 0 <= idx <= 2m  */
            if (idx > N) break;                /* outside required range */
            sum[idx] = sum[idx] + cur[k];
        }

        /* prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        prev_len = m + 1;
    }

    return sum[N];
}
