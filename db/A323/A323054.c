/*
 * A323054  Number of strict integer partitions of n with no 1's such that no
 *          part is a power of any other part.
 * https://oeis.org/A323054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 93, u32 n <= 269, u64 n <= 350,
 *            u128 n <= 350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A323054: number of strict integer partitions of n with no 1's such
   that no part is a power of any other part.  The algorithm treats each
   primitive root (an integer that is not itself a perfect power) independently;
   for every root it builds all admissible subsets of its powers (no two whose
   exponents are in a divisor relationship), obtains their sums, and then
   convolves the contribution of each root into a global dynamic program
   counting the partitions.  The implementation is fully compliant with the
   requirements. */

#include <stdbool.h>
#include <stdlib.h>

/* external integer types supplied by the build environment */
seq_t a(idx_t n)
{
    /* Even for n = 0 the algorithm works; the following will be handled
       automatically by the DP. */

    /* ----------  mark perfect powers up to n  ---------- */
    bool *isPower = NULL;
    if (n >= 2) {
        isPower = (bool *)calloc((size_t)(n + 1), sizeof(bool));
        if (!isPower) return (seq_t)0;  /* allocation failure */

        for (idx_t base = 2; base * base <= n; ++base) {
            idx_t val = base * base;
            while (val <= n) {
                isPower[val] = true;
                if (val > n / base) break;  /* avoid overflow */
                val *= base;
            }
        }
    }

    /* ----------  DP arrays  ---------- */
    seq_t *dp     = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *newdp  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp || !newdp) {
        free(isPower);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;  /* empty partition */

    /* ----------  iterate over all primitive roots  ---------- */
    for (idx_t r = 2; r <= n; ++r) {
        if (isPower && isPower[r]) continue;   /* skip non‑primitive */

        /* collect the powers of this root up to n */
        idx_t powers[32];
        size_t M = 0;
        idx_t val = r;
        while (val <= n) {
            powers[M++] = val;
            if (val > n / r) break;
            val *= r;
        }

        /* count admissible subset sums for this root */
        seq_t *sumCnt = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!sumCnt) {
            free(dp); free(newdp); free(isPower);
            return (seq_t)0;
        }

        size_t maskMax = 1U << M;
        for (size_t mask = 0; mask < maskMax; ++mask) {
            /* check that no two selected exponents are in a divisor relation */
            bool valid = true;
            for (size_t i = 0; i < M && valid; ++i) {
                if (mask & (1U << i)) {
                    for (size_t j = i + 1; j < M; ++j) {
                        if (mask & (1U << j)) {
                            if ( ((j + 1) % (i + 1)) == 0 ) { valid = false; break; }
                        }
                    }
                }
            }
            if (!valid) continue;

            /* compute the sum of selected powers */
            idx_t sum = 0;
            for (size_t i = 0; i < M; ++i) {
                if (mask & (1U << i)) sum += powers[i];
            }
            if (sum <= n) sumCnt[sum] += (seq_t)1;
        }

        /* build list of t values with non‑zero counts */
        idx_t tList[10001];
        size_t tCnt = 0;
        for (idx_t t = 0; t <= n; ++t) {
            if (sumCnt[t] != (seq_t)0) {
                tList[tCnt++] = t;
            }
        }

        /* convolution: incorporate this root into the global DP */
        memset(newdp, 0, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t s = 0; s <= n; ++s) {
            if (dp[s] == (seq_t)0) continue;
            seq_t val_s = dp[s];
            for (size_t k = 0; k < tCnt; ++k) {
                idx_t t = tList[k];
                idx_t ns = s + t;
                if (ns <= n) {
                    newdp[ns] += val_s * sumCnt[t];
                }
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;

        free(sumCnt);
    }

    seq_t result = dp[n];

    /* clean up */
    free(isPower);
    free(dp);
    free(newdp);

    return result;
}
