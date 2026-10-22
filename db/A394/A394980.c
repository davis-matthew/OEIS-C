/*
 * A394980  Number of 2n-digit decimal strings where the second half is an
 *          anagram of the first half.
 * https://oeis.org/A394980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A287316(n,10) = \sum_{k=0}^n \binom{n}{k}^2 a_{9}(k) with
 *            a_1(k)=1 for all k. This recurrence gives the number of 2n‑digit
 *            decimal strings whose second half is an anagram of the first
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Length of the working arrays */
    size_t len = (size_t)n + 1;

    /* Allocate memory for two arrays of seq_t */
    seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *nxt = (seq_t *)malloc(len * sizeof(seq_t));
    if (!cur || !nxt) {
        /* Allocation failure – return 0 as a safe default */
        free(cur);
        free(nxt);
        return (seq_t)0;
    }

    /* Base case for alphabet of size 1: a_1(k)=1 for all k */
    for (idx_t i = 0; i <= n; ++i) {
        cur[i] = (seq_t)1;
    }

    /* Iteratively add letters up to alphabet size 10 */
    for (int letter = 2; letter <= 10; ++letter) {
        for (idx_t k = 0; k <= n; ++k) {
            seq_t sum = (seq_t)0;

            /* binom(k,i) for i = 0 .. k */
            seq_t binom = (seq_t)1; /* binom(k,0) */
            for (idx_t i = 0; i <= k; ++i) {
                seq_t term = cur[i];
                term = term * binom;     /* multiply by binom(k,i) */
                term = term * binom;     /* multiply by binom(k,i) again (square) */
                sum += term;             /* accumulate */

                if (i < k) {
                    /* Update to binom(k,i+1) = binom(k,i) * (k-i)/(i+1) */
                    seq_t num = (seq_t)(k - i);
                    seq_t den = (seq_t)(i + 1);
                    binom = (binom * num) / den;
                }
            }
            nxt[k] = sum;
        }
        /* Swap cur and nxt for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = cur[n];
    free(cur);
    free(nxt);
    return result;
}
