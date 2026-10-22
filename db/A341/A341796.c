/*
 * A341796  Number of ways to write n as an ordered sum of 5 nonzero
 *          tetrahedral numbers.
 * https://oeis.org/A341796
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∑_{k≥1} x^{k(k+1)(k+2)/6})^5 i.e.,
 *            the number of ordered 5‑tuples of nonzero tetrahedral numbers
 *            whose sum is n
 * Range:     u8 n <= 154, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n below the offset simply return 0. */
    if (n < 5) {
        return (seq_t)0;
    }

    /* Determine how many tetrahedral numbers are ≤ n.
       T_k = k(k+1)(k+2)/6.  k is always small for n ≤ 10000. */
    idx_t maxk = 1;
    while ( (maxk + 1) * (maxk + 2) * (maxk + 3) / 6 <= n ) {
        ++maxk;
    }

    idx_t tcnt = maxk;          /* number of tetrahedral values */
    idx_t *tet = (idx_t *)calloc((size_t)tcnt, sizeof(idx_t));

    idx_t k;
    for (k = 1; k <= maxk; ++k) {
        tet[k - 1] = k * (k + 1) * (k + 2) / 6;
    }

    /* dp_prev[sum] = number of ways to reach `sum` using the current
       number of terms.  All arrays have size n+1. */
    seq_t *dp_prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp_cur  = NULL;

    /* Initialise for one term: each tetrahedral number contributes
       one way to reach its value. */
    for (k = 0; k < tcnt; ++k) {
        if (tet[k] <= n) {
            dp_prev[tet[k]] = 1;
        }
    }

    /* Add terms one by one until we have exactly five terms. */
    for (idx_t iter = 2; iter <= 5; ++iter) {
        dp_cur = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        for (idx_t sum = 1; sum <= n; ++sum) {
            seq_t ways = dp_prev[sum];
            if (!ways) {
                continue;
            }
            for (k = 0; k < tcnt; ++k) {
                idx_t ns = sum + tet[k];
                if (ns <= n) {
                    dp_cur[ns] += ways;
                }
            }
        }
        free(dp_prev);
        dp_prev = dp_cur;
    }

    seq_t result = dp_prev[n];

    free(dp_prev);
    free(tet);

    return result;
}
