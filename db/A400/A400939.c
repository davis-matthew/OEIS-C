/*
 * A400939  The halving Recaman walk: a(1) = 1; for n > 1, a(n) = a(n-1)/2 if
 *          a(n-1) is even and a(n-1)/2 has not already appeared, otherwise
 *          a(n) = a(n-1) + n.
 * https://oeis.org/A400939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, new
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1)/2 if a(n-1) is even and
 *            a(n-1)/2 has not appeared before; otherwise a(n) = a(n-1) + n
 * Range:     u8 n <= 69, u16 n <= 8972, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* computed a(k) values */
static size_t  cache_cap = 0;   /* allocated capacity of cache */
static size_t  cache_len = 0;   /* number of values stored in cache */

static seq_t *visited = NULL;   /* all a(k) values seen so far (unordered) */
static size_t  visited_cap = 0;
static size_t  visited_len = 0;

/* Ensure all terms up to index n are computed */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;          /* nothing to do */

    /* initial allocation if necessary */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 1;
        cache[0] = (seq_t)1;                     /* a(1) */

        visited_cap = 16;
        visited = (seq_t*)malloc(sizeof(seq_t) * visited_cap);
        visited_len = 1;
        visited[0] = cache[0];
    }

    /* compute terms until we reach n */
    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        seq_t next;

        /* does prev even?  use modulus to avoid potential lack of & on big ints */
        if ((prev % (seq_t)2) == 0) {
            seq_t half = prev / (seq_t)2;

            /* linear search for half in visited list */
            int found = 0;
            for (size_t i = 0; i < visited_len; ++i) {
                if (visited[i] == half) { found = 1; break; }
            }

            if (!found)
                next = half;                         /* halve if not seen */
            else
                next = prev + (seq_t)(cache_len + 1); /* otherwise add n */
        } else {
            next = prev + (seq_t)(cache_len + 1);     /* odd: add n */
        }

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_len++] = next;

        /* grow visited array if necessary */
        if (visited_len >= visited_cap) {
            visited_cap *= 2;
            visited = (seq_t*)realloc(visited, sizeof(seq_t) * visited_cap);
        }
        visited[visited_len++] = next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1; return 0 just in case */

    ensure_cache(n);
    return cache[n - 1];
}
