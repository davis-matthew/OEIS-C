/*
 * A345592  Numbers that are the sum of nine fourth powers in eight or more
 *          ways.
 * https://oeis.org/A345592
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

/* a(n) = n-th number that can be expressed as the sum of nine fourth
   powers in at least eight distinct unordered ways.
   This implementation builds the counts of representations by a
   dynamic programming over the fourth powers up to a growing bound.
   Counts are capped at eight to keep the byte array small.
   The function memoises the generated sequence so that multiple
   calls are efficient, and recomputes it only when more terms are needed. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of borrowed utilities (unused here, only shown for
   compatibility with the build system). */
extern idx_t _floor_isqrt_u64(idx_t n);

/* Return floor( n^(1/4) ) for n <= 2^64-1. */
static inline uint64_t floor_fourth_root(uint64_t n)
{
    uint64_t r = 0;
    while (true)
    {
        uint64_t next = r + 1;
        unsigned __int128 p = (unsigned __int128)next * next * next * next;
        if (p > n) break;
        r = next;
    }
    return r;
}

/* Global caches for sequence values and the bound that was used. */
static seq_t  *seq_cache       = NULL; /* array of sequence terms */
static size_t  seq_cache_len   = 0;    /* number of terms stored */
static size_t  seq_cache_bound = 0;    /* upper bound used for generation */

/* Compute the sequence up to at least nth term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not part of OEIS offset */

    /* If we already have enough terms, return the cached value. */
    if (seq_cache_len >= (size_t)n)
        return seq_cache[(size_t)n - 1];

    /* Determine a new bound to generate until we have at least n terms. */
    size_t bound = seq_cache_bound ? seq_cache_bound : 200000; /* start */
    while (true)
    {
        /* Allocate DP arrays: dp[k][s] for k = 0..9, s = 0..bound */
        unsigned char **dp = (unsigned char **)malloc(10 * sizeof(*dp));
        if (!dp) exit(EXIT_FAILURE);
        for (int k = 0; k <= 9; ++k)
        {
            dp[k] = (unsigned char *)calloc(bound + 1, sizeof(unsigned char));
            if (!dp[k]) { /* clean up on failure */
                for (int j = 0; j < k; ++j) free(dp[j]);
                free(dp);
                exit(EXIT_FAILURE);
            }
        }
        dp[0][0] = 1; /* one way to sum to 0 with 0 numbers */

        uint64_t max_i = floor_fourth_root(bound);
        for (uint64_t i = 1; i <= max_i; ++i)
        {
            unsigned __int128 v128 = (unsigned __int128)i * i * i * i;
            unsigned int v = (unsigned int)v128; /* v <= bound, so fits in uint32 */
            /* Update DP in increasing k to allow unlimited use of the same v */
            for (int k = 1; k <= 9; ++k)
            {
                for (size_t s = v; s <= bound; ++s)
                {
                    unsigned int add = dp[k - 1][s - v];
                    if (add)
                    {
                        unsigned int sum = dp[k][s] + add;
                        dp[k][s] = (unsigned char)(sum > 8 ? 8 : sum);
                    }
                }
            }
        }

        /* Collect numbers with at least 8 representations. */
        seq_t *new_terms = NULL;
        size_t new_len = 0;
        size_t new_cap = 0;
        for (size_t s = 1; s <= bound; ++s)
        {
            if (dp[9][s] >= 8)
            {
                if (new_len == new_cap)
                {
                    new_cap = new_cap ? new_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(new_terms,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) { /* clean up before exiting */
                        for (int k = 0; k <= 9; ++k) free(dp[k]);
                        free(dp);
                        free(new_terms);
                        exit(EXIT_FAILURE);
                    }
                    new_terms = tmp;
                }
                new_terms[new_len++] = (seq_t)s;
            }
        }

        /* Free DP arrays */
        for (int k = 0; k <= 9; ++k) free(dp[k]);
        free(dp);

        if (new_len >= (size_t)n)
        {
            /* Replace old cache with the new one. */
            free(seq_cache);
            seq_cache = new_terms;
            seq_cache_len = new_len;
            seq_cache_bound = bound;
            break;
        }
        /* Not enough terms: double the bound and try again. */
        free(new_terms);
        bound <<= 1;
    }

    return seq_cache[(size_t)n - 1];
}
