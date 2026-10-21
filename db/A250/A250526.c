/*
 * A250526  Number of (n+1)X(7+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) -85*a(n-2) +294*a(n-3) -639*a(n-4) +906*a(n-5)
 *            -839*a(n-6) +490*a(n-7) -164*a(n-8) +24*a(n-9) (n > 9) with
 *            initial values: a(1)=112373, a(2)=682921, a(3)=2771901,
 *            a(4)=9240559, a(5)=27710543, a(6)=78204775, a(7)=212707851,
 *            a(8)=565044857, a(9)=1478308633 Offset: 1 (a(1) is the fi...
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 31, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial terms (index 1..9) */
static const seq_t init_terms[9] =
{
    (seq_t)112373ULL,
    (seq_t)682921ULL,
    (seq_t)2771901ULL,
    (seq_t)9240559ULL,
    (seq_t)27710543ULL,
    (seq_t)78204775ULL,
    (seq_t)212707851ULL,
    (seq_t)565044857ULL,
    (seq_t)1478308633ULL
};

/* Cache of computed terms */
static seq_t *cache    = NULL;
static size_t cache_sz = 0;

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not defined for offset 0 */

    /* Ensure cache at least contains the first 9 terms */
    if (cache_sz == 0)
    {
        cache_sz = 9;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init_terms[i];
    }

    /* Expand cache if necessary */
    if ((size_t)n > cache_sz)
    {
        size_t new_sz = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        /* Compute terms from cache_sz to new_sz-1 */
        for (size_t i = cache_sz; i < new_sz; ++i)
        {
            /* Use recurrence:
               a[i] = a[i-1]*14 - a[i-2]*85 + a[i-3]*294 ... + a[i-9]*24
            */
            seq_t sum_pos = cache[i-1]*14
                            + cache[i-3]*294
                            + cache[i-5]*906
                            + cache[i-7]*490
                            + cache[i-9]*24;
            seq_t sum_neg = cache[i-2]*85
                            + cache[i-4]*639
                            + cache[i-6]*839
                            + cache[i-8]*164;
            cache[i] = sum_pos - sum_neg;
        }
        cache_sz = new_sz;
    }

    return cache[(size_t)n - 1];
}
