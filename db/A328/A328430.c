/*
 * A328430  Number of inversion sequences of length n avoiding the consecutive
 *          patterns 001 and 012.
 * https://oeis.org/A328430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A328430
   Number of inversion sequences of length n avoiding
   consecutive patterns 001 and 012
   (see OEIS entry A328430)
*/

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#define SEQ_ALLOC(n) (new seq_t[(n)]())
#define SEQ_FREE(p)  delete[] (p)
#else
#define SEQ_ALLOC(n) ((seq_t*)calloc((n), sizeof(seq_t)))
#define SEQ_FREE(p)  free(p)
#endif

/* Return a(n) for the sequence A328430. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u || n == 1u)
        return (seq_t)1;

    /* Work with sizes that fit in size_t */
    size_t N = (size_t)n;

    /* Allocate DP arrays */
    seq_t *dp0_prev = SEQ_ALLOC(N + 1); /* sequences ending with flag 0 (last step a drop) */
    seq_t *dp1_prev = SEQ_ALLOC(N + 1); /* sequences ending with flag 1 (last step non‑decreasing) */
    seq_t *dp0_new  = SEQ_ALLOC(N + 1);
    seq_t *dp1_new  = SEQ_ALLOC(N + 1);

    /* Initialize for length 1: only sequence 0 with flag 0 */
    dp0_prev[0] = (seq_t)1;
    dp1_prev[0] = (seq_t)0;

    /* DP for lengths 2 .. n */
    for (size_t k = 2; k <= N; ++k)   /* current length k */
    {
        size_t len_prev = k - 1;       /* previous length */
        /* Compute totals of previous counts */
        seq_t total0 = 0, total1 = 0;
        for (size_t i = 0; i < len_prev; ++i)
        {
            total0 += dp0_prev[i];
            total1 += dp1_prev[i];
        }

        /* Prefix sums of previous counts */
        seq_t pref0 = 0, pref1 = 0;
        for (size_t j = 0; j < k; ++j)   /* new indices 0 .. k-1 */
        {
            seq_t cp0, cp1;
            if (j < len_prev)
            {
                cp0 = dp0_prev[j];
                cp1 = dp1_prev[j];
            }
            else
            {
                cp0 = (seq_t)0;
                cp1 = (seq_t)0;
            }

            /* dp1_new[j] = cp1 + sum_{prev <= j} dp0_prev[prev] */
            dp1_new[j] = cp1 + pref0 + cp0;

            /* dp0_new[j] = (total0 + total1) - sum_{prev <= j} (cp0 + cp1) */
            dp0_new[j] = (total0 + total1) - (pref0 + cp0 + pref1 + cp1);

            /* Update prefixes for next iteration */
            pref0 += cp0;
            pref1 += cp1;
        }

        /* Swap new and previous arrays for next step */
        seq_t *tmp = dp0_prev; dp0_prev = dp0_new; dp0_new = tmp;
        tmp = dp1_prev;        dp1_prev = dp1_new;  dp1_new = tmp;
    }

    /* Sum all counts for length n */
    seq_t result = 0;
    for (size_t i = 0; i < N; ++i)
    {
        result += dp0_prev[i];
        result += dp1_prev[i];
    }

    /* Free temporary arrays */
    SEQ_FREE(dp0_prev);
    SEQ_FREE(dp1_prev);
    SEQ_FREE(dp0_new);
    SEQ_FREE(dp1_new);

    return result;
}
