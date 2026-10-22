/*
 * A335730  a(n) is the number of smallest parts in the overpartitions of n
 *          having odd smallest part.
 * https://oeis.org/A335730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts smallest parts in overpartitions of n with odd
 *            smallest part. Computation is based on the recurrence tot[n][m]
 *            = tot[n][m+1] + 2*sum_{k>=1} tot[n - k*m][m+1] and the weighted
 *            sum contrib[n][m] = 2*sum_{k>=1} k * tot[n - k*m][m+1] The
 *            arrays tot (for all m) and the accumulated answer a...
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *ans_cache = NULL;   /* cached answers up to current maximum n */
static idx_t    computed_max = 0;

/* Compute sequence up to needed n and store in ans_cache. */
static void compute_up_to(idx_t needed)
{
    size_t sz = (size_t)needed + 1;
    seq_t *old_tot = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *new_tot = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *ans_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!old_tot || !new_tot || !ans_arr) {
        free(old_tot); free(new_tot); free(ans_arr);
        return;
    }

    /* initialise tot for m = needed+1 (no parts available) */
    old_tot[0] = (seq_t)1;
    for (idx_t i = 1; i <= needed; ++i) old_tot[i] = 0;
    for (idx_t i = 0; i <= needed; ++i) ans_arr[i] = 0;

    /* iterate m from needed down to 1 */
    for (idx_t m = needed; m >= 1; --m) {
        for (idx_t d = 0; d < m; ++d) {
            seq_t S1 = 0;   /* sum of old_tot over earlier t */
            seq_t S2 = 0;   /* sum of t*old_tot over earlier t (t from 0) */
            for (idx_t t = 0; ; ++t) {
                idx_t idx = d + t * m;
                if (idx > (idx_t)needed) break;
                seq_t old_val = old_tot[idx];
                new_tot[idx] = old_val + (seq_t)2 * S1;
                if (m & 1U) { /* odd m contributes to answer */
                    ans_arr[idx] += (seq_t)2 * (((seq_t)t * S1) - S2);
                }
                S1 += old_val;
                S2 += (seq_t)t * old_val;
            }
        }
        /* prepare for next m: old_tot becomes new_tot */
        seq_t *tmp = old_tot;
        old_tot = new_tot;
        new_tot = tmp;
    }

    /* free temporaries and store result */
    free(old_tot);
    free(new_tot);
    free(ans_cache);        /* discard previous cache if any */
    ans_cache   = ans_arr;  /* new cache */
    computed_max = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > computed_max) compute_up_to(n);
    return ans_cache[n];
}
