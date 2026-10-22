/*
 * A345780  Numbers that are the sum of seven cubes in exactly eight ways.
 * https://oeis.org/A345780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 343, u32 n <= 343, u64 n <= 343, u128 n <= 343,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the nth number that can be expressed as a sum of seven
 * positive integer cubes in exactly eight unordered ways.
 * 
 * The function builds the sequence by repeatedly generating all
 * seven‑cube sums up to an increasing bound and collecting those sums
 * that occur exactly eight times.  This is a classic unbounded
 * knapsack DP over cube values; the DP array stores, for each k
 * (0 ≤ k ≤ 7), how many ways a given sum can be written using exactly k cubes.
 * Only the count up to 9 is kept (capped) because we only need to know
 * whether the number of representations equals eight.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The required interface ---------------------------------------------------- */
seq_t a(idx_t n);

/* Local static state for memoisation ---------------------------------------- */
static seq_t *terms      = NULL;   /* array with the terms of the sequence   */
static size_t terms_len   = 0;     /* number of stored terms                 */
static size_t current_maxS = 0;    /* highest sum that has been examined     */

/* --------------------------------------------------------------------------- */
/* Build all seven‑cube sums up to maxS, collect those occurring exactly 8
 * times, and store them in the global terms array.
 * --------------------------------------------------------------------------- */
static void recompute_terms(size_t maxS)
{
    const size_t stride = maxS + 1;              /* one row per sum value   */
    const size_t rows   = 8;                     /* k = 0..7                */
    const size_t total  = rows * stride;         /* total dp cells          */

    /* dp will be a 1‑D array; index = k*stride + s */
    unsigned char *dp = (unsigned char *)calloc(total, 1);
    if (!dp) abort();                            /* allocation failure      */

    dp[0] = 1;                                   /* 0 cubes sum to 0          */

    /* Iterate over all distinct cube values that fit in the bound.  */
    for (unsigned int c = 1;; ++c) {
        unsigned long long cubeLL = (unsigned long long)c * c * c;
        if (cubeLL > maxS) break;
        const unsigned int cube = (unsigned int)cubeLL;

        /* For each cube, allow it to be used an unlimited number of times.
         * The loops must run with k increasing and s increasing to permit
         * multiple copies of the same cube while keeping the combinations
         * unordered.  */
        for (unsigned k = 1; k <= 7; ++k) {
            unsigned char *rowk   = dp + k * stride;
            unsigned char *rowkm1 = dp + (k - 1) * stride;
            for (size_t s = cube; s <= maxS; ++s) {
                unsigned char add = rowkm1[s - cube];
                if (!add) continue;         /* nothing to add            */
                unsigned int old = rowk[s];
                unsigned int newv = old + add;
                if (newv > 9) newv = 9;      /* cap to avoid overflow    */
                rowk[s] = (unsigned char)newv;
            }
        }
    }

    /* Collect all sums having exactly eight representations. */
    size_t cap = 128;
    seq_t *newTerms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!newTerms) { free(dp); abort(); }

    size_t len = 0;
    unsigned char *row7 = dp + 7 * stride;

    for (size_t s = 1; s <= maxS; ++s) {
        if (row7[s] == 8) {
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(newTerms, cap * sizeof(seq_t));
                if (!tmp) { free(newTerms); free(dp); abort(); }
                newTerms = tmp;
            }
            newTerms[len++] = (seq_t)s;
        }
    }

    free(dp);

    /* Replace the old cache with the new one. */
    free(terms);
    terms      = newTerms;
    terms_len  = len;
    current_maxS = maxS;
}

/* --------------------------------------------------------------------------- */
/* Public function ----------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for index 0 – return 0 */

    /* Initial build with a modest bound. */
    if (!terms) {
        recompute_terms(200000);     /* initial bound; grows if needed   */
    }

    /* Grow the bound until the cache contains at least n terms. */
    while (terms_len < (size_t)n) {
        size_t new_maxS = current_maxS * 2;
        recompute_terms(new_maxS);
    }

    return terms[n - 1];
}
