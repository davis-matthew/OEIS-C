/*
 * A345843  Numbers that are the sum of nine fourth powers in exactly one
 *          ways.
 * https://oeis.org/A345843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6232, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: count integers that are the sum of nine fourth powers in exactly
   one way.  The routine constructs a “coin‑change” dynamic‑programming
   table for fourth‑powers up to a sufficient limit and extracts the
   numbers that have a single representation.  The resulting list is
   cached so that subsequent calls can return the n‑th entry in O(1). */

#include <stdlib.h>
#include <stdint.h>

static size_t cache_limit = 0;   /* last limit used for precomputation   */
static seq_t *cache_terms = NULL;/* array of sequence values            */
static size_t cache_len   = 0;   /* number of terms computed            */
static int    cache_ready = 0;   /* flag: have we computed all terms?   */

/* Ensure that at least n terms have been computed and cached. */
static void build_terms(idx_t n)
{
    if (cache_ready && cache_len >= (size_t)n)
        return;

    size_t limit = 200000;               /* initial bound on sums           */

    while (1) {
        size_t sz = limit + 1;            /* we need indices 0..limit        */

        /* dp[k][s] holds the number of ways to reach sum s with exactly k terms.
           Only the values 0,1,2 are needed, so a byte per entry suffices. */
        uint8_t *dp = (uint8_t *)calloc(10u * sz, sizeof(uint8_t));
        if (!dp)
            exit(1);                     /* allocation failure */

        dp[0] = 1;                        /* one way to form 0 with 0 terms */

        /* Process all fourth powers that fit in the table. */
        for (uint64_t base = 1; ; ++base) {
            uint64_t val = base * base;   /* base² */
            val = val * base;            /* base³ */
            val = val * base;            /* base⁴ */
            if (val > limit)
                break;
            size_t v = (size_t)val;

            /* For each k = 1..9, update dp[k][s] using dp[k-1]**. */
            for (size_t k = 1; k <= 9; ++k) {
                uint8_t *prev = dp + (k - 1) * sz;
                uint8_t *cur  = dp + k * sz;
                for (size_t s = v; s <= limit; ++s) {
                    uint8_t add = prev[s - v];
                    if (!add)
                        continue;
                    uint8_t tot = cur[s] + add;
                    cur[s] = (tot > 2 ? 2 : tot);
                }
            }
        }

        /* Extract sums with exactly one representation using 9 terms. */
        size_t cap = 0, len = 0;
        seq_t *terms = NULL;

        for (size_t s = 9; s <= limit; ++s) {    /* smallest sum with 9 terms is 9*1⁴ = 9 */
            if (dp[9 * sz + s] == 1) {
                if (len == cap) {
                    cap = (cap == 0 ? 128 : cap * 2);
                    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                    if (!tmp) {
                        free((void *)dp);
                        exit(1);
                    }
                    terms = tmp;
                }
                terms[len++] = (seq_t)s;
            }
        }

        free((void *)dp);

        if (len >= (size_t)n) {          /* we have enough terms */
            if (cache_terms)
                free(cache_terms);
            cache_terms = terms;
            cache_len   = len;
            cache_limit = limit;
            cache_ready = 1;
            break;
        }

        /* Not enough terms: discard and expand the search limit. */
        free(terms);
        limit <<= 1;                      /* double the limit */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    build_terms(n);
    return (n <= cache_len) ? cache_terms[n - 1] : 0;
}
