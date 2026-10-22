/*
 * A357455  Number of compositions (ordered partitions) of n into pentanacci
 *          numbers 1,2,4,8,16,31, ... (A001591).
 * https://oeis.org/A357455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p∈P, p≤n} a(n-p), with a(0)=1 P = pentanacci numbers
 *            1,2,4,8,16,31, ... This is implemented by dynamic programming
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External interface -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state -------------------------------------------------------- */
    static seq_t *dp      = NULL;    /* dp[i] = a(i) */
    static size_t dp_size = 0;       /* number of allocated elements */
    static idx_t *pent    = NULL;    /* list of pentanacci numbers */
    static size_t pent_cap = 0;      /* capacity of pent array */
    static size_t pent_cnt = 0;      /* number of pent numbers stored */

    /* --------------------------------------------------------------------- */
    /* Ensure dp array holds indices 0..n (size n+1) */
    if (dp_size < (size_t)n + 1) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        dp = tmp;
        dp_size = new_size;
        /* new elements are uninitialised; they will be computed below */
    }

    /* Base value a(0) = 1 */
    dp[0] = (seq_t)1;

    /* --------------------------------------------------------------------- */
    /* Initialise pentanacci numbers if this is the first call */
    if (pent_cnt == 0) {
        pent_cap = 16;                     /* arbitrary initial capacity */
        pent     = (idx_t *)malloc(pent_cap * sizeof(idx_t));
        if (!pent) return (seq_t)0;
        /* first five pentanacci numbers */
        pent[0] = 1; pent[1] = 2; pent[2] = 4;
        pent[3] = 8; pent[4] = 16;
        pent_cnt = 5;
    }

    /* Extend the pentanacci table until the last entry exceeds n */
    while (pent[pent_cnt - 1] <= n) {
        /* Grow pent array if necessary */
        if (pent_cnt == pent_cap) {
            size_t new_cap = pent_cap * 2;
            idx_t *tmp = (idx_t *)realloc(pent, new_cap * sizeof(idx_t));
            if (!tmp) return (seq_t)0;
            pent = tmp;
            pent_cap = new_cap;
        }
        /* Next pentanacci number = sum of previous five */
        idx_t next = pent[pent_cnt - 1] + pent[pent_cnt - 2]
                   + pent[pent_cnt - 3] + pent[pent_cnt - 4]
                   + pent[pent_cnt - 5];
        pent[pent_cnt++] = next;
    }

    /* --------------------------------------------------------------------- */
    /* Compute DP values from 1 up to n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < pent_cnt && pent[j] <= i; ++j) {
            sum += dp[i - pent[j]];
        }
        dp[i] = sum;
    }

    /* --------------------------------------------------------------------- */
    return dp[n];
}
