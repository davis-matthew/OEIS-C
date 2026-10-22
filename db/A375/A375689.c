/*
 * A375689  Expansion of e.g.f. 1 / (1 + 3 * x * log(1 - x))^(2/3).
 * https://oeis.org/A375689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A375689: a(n) = n! * sum_{k=0..floor(n/2)} (product_{j=0..k-1} (3*j+2)) *
   |Stirling1(n-k,k)|/(n-k)! 
   The implementation iterates over all rows of unsigned Stirling numbers of the first kind,
   adds their contributions to a[n] directly and uses only integer arithmetic.
*/
#include <stdlib.h>   /* calloc, free */

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Convert to size_t for safe array indexing. */
    size_t maxN = (size_t)n;
    size_t maxK = maxN / 2 + 1;               /* we need k up to floor(n/2)  */

    /* Allocate all working arrays (zero‑initialized). */
    seq_t *aArr  = (seq_t *)calloc(maxN + 1, sizeof(seq_t));
    seq_t *cur   = (seq_t *)calloc(maxK + 1, sizeof(seq_t));
    seq_t *next  = (seq_t *)calloc(maxK + 1, sizeof(seq_t));
    seq_t *P     = (seq_t *)calloc(maxK + 1, sizeof(seq_t));
    if (!aArr || !cur || !next || !P) {
        /* Allocation failed: return 0 as a safe fallback. */
        free(aArr); free(cur); free(next); free(P);
        return (seq_t)0;
    }

    /* Initial Stirling numbers: c(0,0)=1.                           */
    cur[0] = (seq_t)1;

    /* Pre‑compute the product P[k] = Π_{j=0}^{k-1} (3*j+2).          */
    P[0] = (seq_t)1;
    for (size_t k = 1; k <= maxK; ++k)
        P[k] = P[k - 1] * (seq_t)(3 * (k - 1) + 2);

    /* Main double loop: iterate over all rows m = 0..maxN.        */
    for (size_t m = 0; m <= maxN; ++m) {
        seq_t f = (seq_t)1;              /* falling factorial part   */

        /* Add contributions from the current Stirling row to a[n]. */
        size_t limitK = (m < maxK) ? m : maxK;   /* valid k are 0..m (and ≤ maxK)       */
        for (size_t k = 0; k <= limitK; ++k) {
            size_t n_val = m + k;
            if (n_val > maxN) break;        /* nothing beyond the requested n   */
            if (cur[k] == (seq_t)0) {       /* skip zero entries                */
                if (k < maxK) f = f * (seq_t)(m + k + 1);
                continue;
            }
            seq_t term = cur[k] * P[k];
            term = term * f;
            aArr[n_val] += term;

            if (k < maxK) f = f * (seq_t)(m + k + 1); /* update f for next k */
        }

        /* Compute the next row of unsigned Stirling numbers.      */
        size_t nextLimit = (m + 1) < maxK ? (m + 1) : maxK;
        next[0] = (seq_t)0;                                  /* c(m+1,0)=0 if m+1>0 */
        for (size_t k = 1; k <= nextLimit; ++k) {
            next[k] = cur[k - 1] + cur[k] * (seq_t)m;        /* recurrence */
        }
        /* Zero out any remaining entries beyond nextLimit.        */
        for (size_t k = nextLimit + 1; k <= maxK; ++k) {
            next[k] = (seq_t)0;
        }

        /* Swap cur and next for the next iteration.              */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Retrieve the desired value and free temporary storage.      */
    seq_t result = aArr[maxN];
    free(aArr); free(cur); free(next); free(P);
    return result;
}
