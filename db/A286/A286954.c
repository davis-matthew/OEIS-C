/*
 * A286954  Number of maximal irredundant sets in the n-cycle graph.
 * https://oeis.org/A286954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 57, u64 n <= 115,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence for OEIS A286954 (Number of maximal irredundant sets in the n‑cycle graph):
 *   a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-5)
 *          - a(n-7) - 2*a(n-8) - a(n-9)
 *          + 2*a(n-10) + a(n-11) - a(n-14)   for n > 14
 * with initial values:
 *   a(1)=0 , a(2)=2 , a(3)=3 , a(4)=6 , a(5)=10 , a(6)=11 ,
 *   a(7)=14 , a(8)=14 , a(9)=30 , a(10)=62 , a(11)=66 ,
 *   a(12)=99 , a(13)=143 , a(14)=212 .
 * The function memoises results, is safe for any order of calls, and works for
 * all supported seq_t / idx_t types.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of computed values */
static seq_t *cache = NULL;      /* cache[0] unused, cache[1..] valid */
static idx_t last_computed = 0;  /* largest n for which cache[n] is valid */

/* Initialise cache up to at least 14 (or to requested n). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* start with room for 14 entries (index 1..14) */
        size_t init_cap = n > 14 ? n + 1 : 15;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed – abort */
            return;
        }
        /* base values (index 0 unused) */
        cache[1]  = (seq_t)0;
        cache[2]  = (seq_t)2;
        cache[3]  = (seq_t)3;
        cache[4]  = (seq_t)6;
        cache[5]  = (seq_t)10;
        cache[6]  = (seq_t)11;
        cache[7]  = (seq_t)14;
        cache[8]  = (seq_t)14;
        cache[9]  = (seq_t)30;
        cache[10] = (seq_t)62;
        cache[11] = (seq_t)66;
        cache[12] = (seq_t)99;
        cache[13] = (seq_t)143;
        cache[14] = (seq_t)212;
        last_computed = 14;
    }

    /* expand cache if necessary */
    if (n <= last_computed) return;
    size_t new_cap = n + 1;
    if (new_cap <= last_computed) return;  /* nothing to do */

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (cache == NULL) return;  /* allocation failed – abort */

    /* compute missing terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence only valid for i > 14, but we will only call this for such i */
        seq_t pos = cache[i - 2] + cache[i - 3];
        pos += cache[i - 4];
        pos += cache[i - 5];
        pos += (cache[i - 10] << 1);
        pos += cache[i - 11];

        seq_t val = pos;
        val -= cache[i - 7];
        val -= (cache[i - 8] << 1);
        val -= cache[i - 9];
        val -= cache[i - 14];

        cache[i] = val;
    }
    last_computed = n;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n=0, but guard anyway */
    if (n <= 14) {                        /* direct lookup for the base cases */
        if (cache == NULL) {               /* initialise if not yet done */
            ensure_cache(14);
        }
        return cache[n];
    }

    ensure_cache(n);
    return cache[n];
}
