/*
 * A328409  Number of inversion sequences of length n where all consecutive
 *          subsequences i,j,k satisfy i > j < k or i <= j >= k.
 * https://oeis.org/A328409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of inversion sequences of length n satisfying the
 *            triple condition i>j<k or i<=j>=k
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* #include <stdint.h> is optional, seq_t and idx_t are provided by the build. */
#include <stdlib.h>

#define NEG 0   /* previous difference was negative          */
#define ZERO 1  /* previous difference was zero              */
#define POS 2   /* previous difference was positive          */

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 0ull) return (seq_t)1;
    if (n == 1ull) return (seq_t)1;

    /* allocate memory for current and next dp states
       each state holds counts for values 0 .. n   (size n+1) */
    seq_t *cur[3], *next[3];
    idx_t sz = n + 1;                     /* maximum value index */
    cur[NEG]   = (seq_t *)calloc(sz, sizeof(seq_t));
    cur[ZERO]  = (seq_t *)calloc(sz, sizeof(seq_t));
    cur[POS]   = (seq_t *)calloc(sz, sizeof(seq_t));
    next[NEG]  = (seq_t *)calloc(sz, sizeof(seq_t));
    next[ZERO] = (seq_t *)calloc(sz, sizeof(seq_t));
    next[POS]  = (seq_t *)calloc(sz, sizeof(seq_t));

    /* initialise state for i = 1 (e1)  */
    cur[ZERO][0] = (seq_t)1;      /* e1 = 0  -> d1 = 0  (zero)  */
    cur[POS][1]  = (seq_t)1;      /* e1 = 1  -> d1 = +1 (positive) */

    /* DP for i = 2 .. n-1  (indices of e) */
    for (idx_t i = 2; i <= n-1; ++i) {
        idx_t M = i - 1;  /* previous maximum value of e[i-1] */

        /* zero the new state arrays for indices 0 .. i */
        for (idx_t u = 0; u <= i; ++u) {
            next[NEG][u] = (seq_t)0;
            next[ZERO][u] = (seq_t)0;
            next[POS][u] = (seq_t)0;
        }

        /* --- compute new POS (new difference > 0) --- */
        seq_t sum_neg = (seq_t)0;
        for (idx_t u = 0; u <= i; ++u) {
            next[POS][u] = sum_neg;            /* sum of cur[NEG][v] for v < u */
            if (u <= M) sum_neg += cur[NEG][u];
        }

        /* --- compute new ZERO (new difference == 0) --- */
        for (idx_t u = 0; u <= i; ++u) {
            if (u <= M)
                next[ZERO][u] = cur[ZERO][u] + cur[POS][u];
            else
                next[ZERO][u] = (seq_t)0;
        }

        /* --- compute new NEG (new difference < 0) --- */
        seq_t run = (seq_t)0;
        /* iterate v from M down to 0: cur[POS]+cur[ZERO] gives values that may
           produce a negative difference in the next step */
        for (int v = (int)M; v >= 0; --v) {
            next[NEG][v] = run;           /* sum of cur[POS][w]+cur[ZERO][w] for w > v */
            run += cur[POS][v] + cur[ZERO][v];
        }
        next[NEG][i] = (seq_t)0;          /* no state can reach value i with a negative diff */

        /* swap cur and next for next iteration */
        seq_t *tmp;
        tmp = cur[NEG];  cur[NEG]  = next[NEG];  next[NEG]  = tmp;
        tmp = cur[ZERO]; cur[ZERO] = next[ZERO]; next[ZERO] = tmp;
        tmp = cur[POS];  cur[POS]  = next[POS];  next[POS]  = tmp;
    }

    /* sum over all final states for value 0 .. n-1 */
    seq_t ans = (seq_t)0;
    for (idx_t u = 0; u <= n-1; ++u) {
        ans += cur[NEG][u] + cur[ZERO][u] + cur[POS][u];
    }

    /* free allocated memory (optional) */
    free(cur[NEG]); free(cur[ZERO]); free(cur[POS]);
    free(next[NEG]); free(next[ZERO]); free(next[POS]);

    return ans;
}
