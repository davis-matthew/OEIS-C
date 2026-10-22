/*
 * A345803  Numbers that are the sum of ten cubes in exactly one ways.
 * https://oeis.org/A345803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 151, u32 n <= 151, u64 n <= 151,
 *            u128 n <= 151, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Numbers that are the sum of ten positive cubes in exactly one unordered way.
   The function counts the number of representations of each integer as the
   sum of 10 cubes using a bounded coin change DP and extracts those with
   exactly one representation.  The sequence is generated lazily and cached
   between calls.  The algorithm loops over cube values and updates a DP
   array of counts capped at 2, so that only the “exactly one” and
   “more than one” cases matter.  */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* DP helper: compute all terms up to a given largest cube index. */
static void compute_terms(idx_t maxCubeIdx, seq_t **out, size_t *len)
{
    /* Largest cube value (fits into size_t). */
    size_t maxCubeVal = (size_t)maxCubeIdx * (size_t)maxCubeIdx * (size_t)maxCubeIdx;
    /* Maximum possible sum of ten cubes. */
    size_t maxSum = 10 * maxCubeVal;

    size_t USIZE = maxSum + 1;                /* entries per used count (sum 0..maxSum) */
    unsigned char *dp = (unsigned char*)calloc((size_t)(11 * USIZE), 1);
    if (!dp) {
        *out = NULL;
        *len = 0;
        return;
    }

    dp[0] = 1;  /* one way to make sum 0 with 0 cubes */

    /* Process each cube value as an unbounded coin. */
    for (idx_t i = 1; i <= maxCubeIdx; ++i) {
        size_t vi = (size_t)i;
        size_t v  = vi * vi * vi;            /* cube value, fits into size_t */
        for (size_t used = 1; used <= 10; ++used) {
            size_t offUsed = used * USIZE;
            size_t offPrev = (used - 1) * USIZE;
            for (size_t sum = v; sum <= maxSum; ++sum) {
                unsigned char add = dp[offPrev + sum - v];
                if (!add) continue;
                unsigned char *dest = dp + offUsed + sum;
                if (*dest < 2) {
                    *dest += add;
                    if (*dest > 2) *dest = 2;
                }
            }
        }
    }

    /* Extract numbers with exactly one representation. */
    size_t capacity = 1024;
    seq_t *terms = (seq_t*)malloc(capacity * sizeof(seq_t));
    size_t termsn = 0;
    for (size_t sum = 10; sum <= maxSum; ++sum) {   /* minimal sum is 10*1^3 = 10 */
        if (dp[10 * USIZE + sum] == 1) {
            if (termsn == capacity) {
                capacity <<= 1;
                terms = (seq_t*)realloc(terms, capacity * sizeof(seq_t));
            }
            terms[termsn++] = (seq_t)sum;
        }
    }

    free(dp);
    *out = terms;
    *len = termsn;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Lazy initialization of the full sequence container. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static idx_t cube_limit = 10;           /* start with cubes up to 10^3 */

    if (n == 0) return 0;   /* out of range, but undefined behaviour */

    while (seq_len < n) {
        seq_t *newseq;
        size_t newlen;
        compute_terms(cube_limit, &newseq, &newlen);
        if (!newseq) return 0;   /* allocation failure */

        if (newlen >= n) {
            free(seq);
            seq = newseq;
            seq_len = newlen;
            break;
        } else {
            free(newseq);
            cube_limit += 5;      /* enlarge cube range and try again */
            if (cube_limit > 100) { /* safety cap */
                cube_limit = 100;
            }
        }
    }

    /* Return the requested term (array is 0‑based). */
    return seq[n - 1];
}
