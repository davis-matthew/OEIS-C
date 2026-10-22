/*
 * A345514  Numbers that are the sum of six cubes in five or more ways.
 * https://oeis.org/A345514
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

/*  A345514 – Numbers that are the sum of six cubes in five or more ways.
    This implementation builds the sequence by dynamic programming:
    for each cube value v it updates a table dp[k][s] = number of
    unordered ways to write s as the sum of k cubes (capped at 5).
    After processing all cube values it collects all sums s with
    dp[6][s] >= 5 into an array.  The algorithm is run lazily on the
    first call and cached for subsequent calls.  The chosen upper
    bound (1 200 000) is large enough to generate more than ten
    thousand terms; the implementation can be tuned if a larger
    range is required. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static storage for the computed terms */
static seq_t *terms = NULL;
static size_t terms_count = 0;
static size_t terms_cap   = 0;
static bool   initialized = false;

/* maximum value to search; adjust if more terms are needed */
#define LIMIT (1200000ULL)

/* build the sequence once on first use */
static void compute_terms(void)
{
    /* list all cube values up to LIMIT */
    idx_t cube_vals[200];
    size_t cube_cnt = 0;
    for (idx_t i = 1; ; ++i) {
        idx_t c = i * i * i;
        if (c > LIMIT) break;
        cube_vals[cube_cnt++] = c;
    }

    /* dp[k][s] holds the number of unordered ways to write s
       as a sum of k cubes (capped at 5).  We allocate 7 rows (k=0..6). */
    uint8_t *dp[7];
    size_t len = (size_t)LIMIT + 1;
    for (int k = 0; k < 7; ++k) {
        dp[k] = (uint8_t *)calloc(len, 1);
        if (!dp[k]) abort();      /* memory allocation failure */
    }
    dp[0][0] = 1;                  /* one way to sum zero cubes to 0 */

    for (size_t ci = 0; ci < cube_cnt; ++ci) {
        size_t v = (size_t)cube_vals[ci];
        for (int k = 1; k <= 6; ++k) {
            uint8_t *row   = dp[k];
            uint8_t *prev  = dp[k-1];
            for (size_t s = v; s <= LIMIT; ++s) {
                uint8_t add = prev[s - v];
                if (add && row[s] < 5) {
                    uint8_t nv = row[s] + add;
                    row[s] = nv > 5 ? 5 : nv;
                }
            }
        }
    }

    /* Allocate array for terms; we expect a few thousand terms. */
    terms_cap = 20000;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) abort();

    for (size_t s = 1; s <= LIMIT; ++s) {
        if (dp[6][s] >= 5) {
            if (terms_count >= terms_cap) {
                terms_cap <<= 1;     /* double the capacity */
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) abort();
            }
            terms[terms_count++] = (seq_t)s;
        }
    }

    /* free the dp tables */
    for (int k = 0; k < 7; ++k) free(dp[k]);

    /* if we still need more terms (n > terms_count), the caller
       will call a() with a larger index; the implementation does
       NOT recompute for larger limits – this function is meant to
       support up to the chosen LIMIT. */
}

/* public interface required by the build environment */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_terms();
        initialized = true;
    }
    if (n == 0) return 0;               /* undefined, return 0 safely */
    if ((size_t)n > terms_count) return 0;   /* out of bounds */
    return terms[n - 1];
}
