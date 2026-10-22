/*
 * A345805  Numbers that are the sum of ten cubes in exactly three ways.
 * https://oeis.org/A345805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are the sum of ten cubes in exactly three ways
 * Range:     u8 n <= 4, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the precomputed terms of the sequence. */
static seq_t *global_terms   = NULL;
static size_t global_len     = 0;   /* number of terms stored */
static uint64_t global_limit = 0;   /* maximum sum value processed */

/* --------------------------------------------------------------------- */
/* Helper: compute all terms up to a given sum limit 'limit'              */
/* --------------------------------------------------------------------- */
static void compute_terms(uint64_t limit)
{
    /* Allocate memory for DP: 11 rows (0..10 cubes) and (limit+1) columns */
    size_t stride = limit + 1;                     /* row stride */
    uint8_t *dp = (uint8_t*)calloc(11 * stride, sizeof(uint8_t));
    if (!dp) return;                              /* out of memory */

    /* Base case: one way to reach sum 0 using 0 cubes. */
    dp[0 * stride + 0] = 1;

    /* Enumerate cubes: 1^3, 2^3, ..., up to cube <= limit */
    for (uint64_t cube_root = 1; ; ++cube_root)
    {
        uint64_t v = cube_root * cube_root * cube_root;
        if (v > limit) break;

        /* For each sum s >= v, update DP rows for using this cube */
        for (uint64_t s = v; s <= limit; ++s)
        {
            /* idx for current sum */
            size_t idx = (size_t)s;

            /* Update for each possible number of cubes used (1..10) */
            for (uint8_t used = 1; used <= 10; ++used)
            {
                uint8_t add = dp[(used-1) * stride + (idx - (size_t)v)];
                if (add == 0) continue;

                uint8_t cur = dp[used * stride + idx];
                uint8_t val = cur + add;
                if (val > 4) val = 4;            /* we only need to know values up to 3 */
                dp[used * stride + idx] = val;
            }
        }
    }

    /* Gather all sums that can be expressed as sum of 10 cubes in exactly 3 ways */
    seq_t *terms = NULL;
    size_t terms_cap = 0;
    size_t terms_len = 0;

    for (uint64_t s = 10; s <= limit; ++s)   /* smallest possible sum is 10*1^3 */
    {
        if (dp[10 * stride + (size_t)s] == 3) {
            if (terms_len == terms_cap) {
                terms_cap = (terms_cap == 0 ? 1024 : terms_cap * 2);
                seq_t *new_terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                if (!new_terms) {                /* allocation failed, clean up */
                    free(dp);
                    free(terms);
                    global_terms = NULL;
                    global_len   = 0;
                    return;
                }
                terms = new_terms;
            }
            terms[terms_len++] = (seq_t)s;
        }
    }

    free(dp);
    /* Replace old global storage with the new one */
    free(global_terms);
    global_terms = terms;
    global_len   = terms_len;
    global_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n=0 */

    /* Initial precomputation if necessary */
    if (!global_terms) compute_terms(200000);

    /* If required term not yet present, recompute with a bigger limit. */
    if ((size_t)n > global_len) {
        uint64_t new_limit = global_limit * 2 + 100000;
        compute_terms(new_limit);
    }

    /* n is guaranteed to be <= global_len after above. */
    return global_terms[n - 1];
}
