/*
 * A337281  a(n) = n*T(n), where T(n) = A000073(n) = n-th tribonacci number.
 * https://oeis.org/A337281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*T(n) where T(0)=0, T(1)=0, T(2)=1 and *
 *            T(k)=T(k-1)+T(k-2)+T(k-3) (tribonacci)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for tribonacci numbers */
static seq_t *tribo_cache = NULL;
static size_t cache_cap = 0;    /* number of allocated slots */
static idx_t computed_upto = 0; /* highest index whose T has been computed */

static void ensure_tribo(idx_t n)
{
    /* initial allocation */
    if (tribo_cache == NULL) {
        size_t needed = (size_t)n + 1;
        /* at least three slots for the base cases */
        if (needed < 3) needed = 3;
        tribo_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!tribo_cache) abort(); /* allocation failure – unlikely */
        tribo_cache[0] = (seq_t)0;
        tribo_cache[1] = (seq_t)0;
        tribo_cache[2] = (seq_t)1;
        cache_cap = needed;
        computed_upto = 2;
    }

    /* nothing to do if the requested index is already computed */
    if (n <= computed_upto) return;

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(tribo_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure – unlikely */
        tribo_cache = tmp;
        cache_cap = new_cap;
    }

    /* compute tribonacci numbers up to n */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        tribo_cache[i] = tribo_cache[i-1] + tribo_cache[i-2] + tribo_cache[i-3];
    }
    computed_upto = n;
}

seq_t a(idx_t n)
{
    ensure_tribo(n);
    return (seq_t)n * tribo_cache[(size_t)n];
}
