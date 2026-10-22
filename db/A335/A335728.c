/*
 * A335728  a(n) is the number of smallest parts in the overpartitions of n
 *          having even smallest part.
 * https://oeis.org/A335728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A335724(n) – A335730(n)
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via generating‑function manipulation, see analysis. */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Early return for zero (offset 1 in the sequence). */
    if (n == 0) return 0;

    /* Static state to cache computed values. */
    static idx_t computed_max = 0;
    static seq_t *oldO = NULL;
    static seq_t *newO = NULL;
    static seq_t *res  = NULL;

    /* Re‑compute up to n if we have not reached it yet. */
    if (n > computed_max) {
        size_t N = (size_t)n;

        /* Free previous caches if any. */
        if (oldO) seq_free(oldO);
        if (newO) seq_free(newO);
        if (res ) seq_free(res );

        /* Allocate new arrays. */
        oldO = seq_alloc_new_array(N + 1);
        newO = seq_alloc_new_array(N + 1);
        res  = seq_alloc_new_array(N + 1);

        /* Initialise arrays. */
        for (idx_t i = 0; i <= N; ++i) {
            oldO[i] = 0;
            newO[i] = 0;
            res [i] = 0;
        }
        oldO[0] = 1;     /* O_{N+1}(q) = 1 */

        /* Main DP loop.  oldO holds O_{t+1}(q) before the iteration. */
        for (idx_t t = N; t >= 1; --t) {
            /* Contributions from smallest part s = t (even). */
            if ((t & 1) == 0 && t >= 2) {
                const idx_t s = t;
                for (idx_t r = 0; r < s; ++r) {
                    seq_t prefixSum   = 0;
                    seq_t weightedSum = 0;

                    for (idx_t pos = r, k = 0; pos <= N; pos += s, ++k) {
                        if (k >= 1) {
                            seq_t contrib = (seq_t)2 * (
                                ((seq_t)k * prefixSum) - weightedSum
                            );
                            res[pos] += contrib;
                        }
                        seq_t a = oldO[pos];
                        prefixSum   += a;
                        weightedSum += (seq_t)k * a;
                    }
                }
            }

            /* Compute O_t(q) into newO from oldO:
               newO[n] = oldO[n] + 2 * sum_{k>=1} oldO[n-k*t]   (mod truncation to N) */
            for (idx_t r = 0; r < t; ++r) {
                seq_t cum = 0;
                for (idx_t pos = r; pos <= N; pos += t) {
                    newO[pos] = oldO[pos] + (seq_t)2 * cum;
                    cum += oldO[pos];
                }
            }

            /* Swap oldO and newO for next iteration. */
            seq_t *tmp   = oldO;
            oldO         = newO;
            newO         = tmp;
        }

        computed_max = N;   /* Cache up to N. */
    }

    /* Return the requested value (res[0] is unused). */
    return res[n];
}
