/*
 * A260131  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 01010101 or 01010111.
 * https://oeis.org/A260131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + a(n-4) for n>5, a(1)=36, a(2)=58,
 *            a(3)=138, a(4)=266, a(5)=472 Computes the OEIS sequence A260131
 *            via memoised recurrence
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 29, u64 n <= 62, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cast helper that works in both C and C++ */
#ifndef __cplusplus
#define CAST_TO_SEQ_T_PTR(p) ((seq_t*)(p))
#else
#define CAST_TO_SEQ_T_PTR(p) reinterpret_cast<seq_t*>(p)
#endif

/* Static cache: index n is stored in cache[n] (unused cache[0]) */
static seq_t *cache      = NULL;   /* array of values */
static size_t cap        = 0;      /* highest index currently allocated (0 for NULL) */
static idx_t max_n       = 0;      /* largest n computed and stored */

/* Ensure the cache can hold up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n <= (idx_t)cap)
        return;

    size_t new_cap = (cap > 0) ? cap << 1 : 8;
    while (new_cap < (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = CAST_TO_SEQ_T_PTR(realloc(cache, (new_cap + 1) * sizeof(seq_t)));
    if (!tmp)
        abort();              /* out of memory */

    cache = tmp;
    cap   = new_cap;
}

/* Initialise the cache with the first five terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* allocate space up to index 8 initially (0..8) */
    cap = 8;
    cache = CAST_TO_SEQ_T_PTR(malloc((cap + 1) * sizeof(seq_t)));
    if (!cache)
        abort();

    cache[0] = (seq_t)0;         /* unused */
    cache[1] = (seq_t)36;
    cache[2] = (seq_t)58;
    cache[3] = (seq_t)138;
    cache[4] = (seq_t)266;
    cache[5] = (seq_t)472;

    max_n = 5;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* behaviour below offset unspecified */

    init_cache();

    if (n > max_n) {
        ensure_capacity(n);
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + 3*a(i-3) + a(i-4) */
            cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 3] + cache[i - 4];
        }
        max_n = n;
    }

    return cache[n];
}
