/*
 * A305037  Number of nX5 0..1 arrays with every element unequal to 0, 1, 2, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence a(n) = a(n-1) + 9*a(n-3) + 2*a(n-4)
 *            - 14*a(n-6) - 4*a(n-7), n > 11 Initial values: a(1)=16, a(2)=8,
 *            a(3)=13, a(4)=26, a(5)=43, a(6)=109, a(7)=319, a(8)=718,
 *            a(9)=1632, a(10)=4351, a(11)=10773 Sequence index starts at 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based array: cache[1] = a(1) */
static idx_t cache_cap = 0;   /* number of pre‑allocated elements */

/* Ensure the cache can hold up to index `need`.  The array grows slowly.  */
static void ensure_cache(idx_t need)
{
    if (cache_cap >= need) return;
    /* grow to a bit more than needed to reduce reallocs */
    size_t new_cap = need + 10;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort the program (no diagnostic output allowed). */
        exit(1);
    }
    cache = new_cache;
    /* initialise new entries with zero */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
    /* set initial terms if this is the first expansion */
    if (cache_cap >= 11 && cache[1] == 0) {
        cache[1]  = (seq_t)16;
        cache[2]  = (seq_t)8;
        cache[3]  = (seq_t)13;
        cache[4]  = (seq_t)26;
        cache[5]  = (seq_t)43;
        cache[6]  = (seq_t)109;
        cache[7]  = (seq_t)319;
        cache[8]  = (seq_t)718;
        cache[9]  = (seq_t)1632;
        cache[10] = (seq_t)4351;
        cache[11] = (seq_t)10773;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */
    ensure_cache(n);
    /* Compute missing terms up to n */
    for (idx_t k = 1; k <= n && cache[k] == 0; ++k);
    for (idx_t k = (k > 0 ? k : 1); k <= n; ++k) {
        if (k >= 1 && k <= 11) continue;  /* already initialised */
        seq_t term = cache[k-1];
        term += (seq_t)9 * cache[k-3];
        term += (seq_t)2 * cache[k-4];
        term -= (seq_t)14 * cache[k-6];
        term -= (seq_t)4 * cache[k-7];
        cache[k] = term;
    }
    return cache[n];
}
