/*
 * A345597  Numbers that are the sum of ten fourth powers in four or more
 *          ways.
 * https://oeis.org/A345597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Algorithm:  Compute sums of ten fourth powers.
    For each n ≥ 1, return the (n‑th) integer that can be expressed
    as the sum of ten fourth powers in at least four distinct ways
    (unordered multisets).  The values are produced by dynamic
    programming that counts, for each sum s, how many
    representations of s using exactly 10 terms exist, capped at 4.
    All terms with a count ≥ 4 are stored; the function returns the
    (n‑1)-th element of that stored list.  No hard‑coded table is used.  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*  External linkage; idx_t is an unsigned integer type (e.g. uint64_t)
    seq_t is the result type (e.g. uint64_t or arbitrary‑precision type). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* array of discovered terms          */
    static size_t term_cnt = 0;   /* number of terms stored             */

    if (n == 0) return 0;         /* out of range; undefined behaviour */

    /*  If we have already enough terms, return immediately. */
    if (terms && n <= term_cnt) return terms[n - 1];

    /*  Rebuild the table until we gather at least n terms.   */
    idx_t limit = 200000;         /* initial guess; increased as needed */
    while (1) {
        size_t stride = (size_t)limit + 1;               /* number of sums      */
        /*  Allocate DP table: 11 rows (k = 0..10) × (limit+1) columns.  */
        uint8_t *dp = (uint8_t *)malloc(11 * stride);
        if (!dp) return 0;   /* memory allocation failure (unlikely) */
        memset(dp, 0, 11 * stride);
        dp[0] = 1;                           /* one way to make 0 with 0 terms */

        /*  COIN‑CHANGE DP:  for each fourth power value v  */
        for (idx_t i = 1;; ++i) {
            idx_t v = i * i * i * i;          /* v = i^4          */
            if (v > limit) break;             /* no larger values  */
            /*  Update states for all k = 1..10  */
            for (size_t k = 1; k <= 10; ++k) {
                uint8_t *dp_k  = dp + k * stride;
                uint8_t *dp_km = dp + (k - 1) * stride;
                size_t start = (size_t)v;
                for (size_t s = start; s <= (size_t)limit; ++s) {
                    uint8_t add = dp_km[s - start];
                    if (add) {
                        uint8_t sum = dp_k[s] + add;
                        dp_k[s] = (sum > 4) ? 4 : sum;   /* cap at 4 */
                    }
                }
            }
        }

        /*  Count how many sums have at least 4 representations.   */
        uint8_t *dp10 = dp + 10 * stride;
        size_t cnt = 0;
        for (size_t s = 1; s <= (size_t)limit; ++s) {
            if (dp10[s] >= 4) ++cnt;
        }

        if (cnt >= n) {      /* we have enough terms for the requested n */
            seq_t *new_terms = (seq_t *)malloc(cnt * sizeof(seq_t));
            if (!new_terms) {
                free(dp);
                return 0;   /* allocation failure */
            }
            size_t idx = 0;
            for (size_t s = 1; s <= (size_t)limit; ++s) {
                if (dp10[s] >= 4) new_terms[idx++] = (seq_t)s;
            }
            free(dp);
            free(terms);     /* discard any previous cache */
            terms = new_terms;
            term_cnt = cnt;
            break;
        }
        /*  Not enough terms: free DP, enlarge limit, and try again. */
        free(dp);
        limit *= 2;
        /*  Guard against overflow in some very rare cases.          */
        if (limit > 20000000) {
            /*  If we still don’t have enough terms, bail out.      */
            /*  This should never happen for the intended N range.   */
            break;
        }
    }

    return terms[n - 1];
}
