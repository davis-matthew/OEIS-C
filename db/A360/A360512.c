/*
 * A360512  Total number of edges after n generations in hexagonal graph
 *          constructed in first quadrant (see Comments in A360501 for precise
 *          definition).
 * https://oeis.org/A360512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=13, a(6)=19,
 *            a(7)=26, a(8)=34 for n>=9: a(n) =
 *            a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+a(n-7) (derived from
 *            the generating function in OEIS A360512)
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   add_seq, sub_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/add_seq.h"
#include "../utils/sub_seq.h"

#ifdef __cplusplus

#else

#endif

/* Cached values of the sequence */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;        /* allocated size (in elements)      */
static idx_t   cache_last = 0;      /* highest index that has been computed */

/* Ensure the cache holds at least n+1 elements. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 32;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);   /* abort on allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Pre‑populate base values (0..8).  After this, cache_last == 8. */
static void init_base(idx_t n)
{
    static const seq_t base[9] = {0,1,2,4,8,13,19,26,34};
    size_t needed = 9;
    if ((idx_t)needed < n + 1) needed = (size_t)n + 1;
    ensure_capacity((idx_t)needed - 1);
    for (idx_t i = 0; i < 9; ++i) cache[i] = base[i];
    cache_last = 8;
}

/* Compute a(k) for all k ≤ n, filling the cache. */
static void compute_up_to(idx_t n)
{
    if (cache == NULL) init_base(n);
    if (n <= cache_last) return;

    ensure_capacity(n);
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        /* recurrence for k >= 9: a(k) = a(k-1)+a(k-2)-a(k-3)+a(k-4)-a(k-5)-a(k-6)+a(k-7) */
        seq_t pos = add_seq(cache[k-1], cache[k-2]);          /* a(k-1)+a(k-2) */
        pos = add_seq(pos, cache[k-4]);                       /* +a(k-4) */
        pos = add_seq(pos, cache[k-7]);                       /* +a(k-7) */
        seq_t val = sub_seq(pos, cache[k-3]);                 /* -a(k-3) */
        val = sub_seq(val, cache[k-5]);                       /* -a(k-5) */
        val = sub_seq(val, cache[k-6]);                       /* -a(k-6) */
        cache[k] = val;
    }
    cache_last = n;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
