/*
 * A345549  Numbers that are the sum of nine cubes in ten or more ways.
 * https://oeis.org/A345549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates values of A345549.  It counts the
   unordered representations of an integer as a sum of nine
   positive cubes, keeping only numbers that have at least ten
   such representations.  The values are computed on demand and
   cached. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *g_terms   = NULL;   /* cached term list            */
static idx_t g_count   = 0;       /* number of cached terms      */
static idx_t g_max_sum = 0;       /* maximum sum considered      */

/* Recomputes the cached term list until it contains at least
   needed_n elements.  The computation grows the maximum sum
   until enough terms are found. */
static void generate_terms(idx_t needed_n)
{
    if (g_terms && g_count >= needed_n)
        return;                           /* already have enough */

    /* free previous cache if any */
    free(g_terms);
    g_terms = NULL;
    g_count = 0;

    /* start with a reasonable upper bound; double until enough terms */
    idx_t max_sum = 500000;
    while (1) {
        size_t stride   = (size_t)(max_sum + 1);
        size_t dp_size  = stride * 10;          /* k = 0..9 */
        uint8_t *dp = (uint8_t *)calloc(dp_size, sizeof(uint8_t));
        if (!dp)
            exit(1);                            /* allocation failure */

        dp[0] = 1;                              /* dp[0][0] = 1 */

        /* Iterate over cube values v^3 that fit in max_sum */
        for (int v = 1; ; ++v) {
            seq_t cube_seq = (seq_t)v * (seq_t)v * (seq_t)v;
            if (cube_seq > max_sum)
                break;
            size_t c = (size_t)cube_seq;

            for (int k = 1; k <= 9; ++k) {
                size_t off_k   = (size_t)k * stride;
                size_t off_km1 = (size_t)(k - 1) * stride;
                for (size_t s = c; s <= max_sum; ++s) {
                    uint8_t prev = dp[off_km1 + (s - c)];
                    uint8_t cur  = dp[off_k + s];
                    uint16_t sum = cur + prev;   /* count ways so far */
                    if (sum > 10)
                        sum = 10;                /* cap to avoid overflow */
                    dp[off_k + s] = (uint8_t)sum;
                }
            }
        }

        /* Count how many sums achieve at least ten representations */
        size_t count = 0;
        size_t off9 = 9 * stride;
        for (size_t s = 0; s <= max_sum; ++s)
            if (dp[off9 + s] >= 10)
                ++count;

        if (count >= needed_n) {
            /* Allocate and fill the term list */
            seq_t *terms = (seq_t *)malloc(count * sizeof(seq_t));
            if (!terms)
                exit(1);
            size_t idx = 0;
            for (size_t s = 0; s <= max_sum; ++s)
                if (dp[off9 + s] >= 10)
                    terms[idx++] = (seq_t)s;      /* idx < count */

            free(dp);
            g_terms   = terms;
            g_count   = count;
            g_max_sum = max_sum;
            return;
        }

        free(dp);
        max_sum *= 2;     /* increase search bound and retry */
    }
}

/* Public routine to obtain the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1, but never called */

    generate_terms(n);
    return g_terms[(size_t)(n - 1)];
}
