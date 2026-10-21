/*
 * A259956  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00001001.
 * https://oeis.org/A259956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259956
   Formula: a(n) = a(n-1) + 2*a(n-2) + 6*a(n-3) + 5*a(n-4) + a(n-5)
               - 2*a(n-6) - 2*a(n-7) - a(n-8)
   Initial values:
   a(1)=67, a(2)=146, a(3)=439, a(4)=1187, a(5)=3227,
   a(6)=8964, a(7)=24719, a(8)=67952, a(9)=187266
   The function is memoised; it works for any n≥1 on any calling order. */

#include <stddef.h>   /* for size_t, NULL */
#include <stdlib.h>   /* for malloc, realloc */
#include <stdbool.h>  /* for bool */

static void _ensure_capacity(idx_t n, seq_t **cache, size_t *cap)
/* Ensure the cache can hold indices 1..n (array size n+1). */
{
    if (*cap < (size_t)n + 1) {
        size_t newcap = (*cap == 0) ? 64 : *cap;
        while (newcap < (size_t)n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – keep cache NULL; caller will return 0. */
            *cache = NULL;
            *cap   = 0;
            return;
        }
        *cache = tmp;
        *cap   = newcap;
    }
}

seq_t a(idx_t n)
{
    /* Cache of computed values, indices 1..last computed. */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static idx_t  last    = 0;

    /* a(0) is outside the offset – return 0. */
    if (n == 0) return (seq_t)0;

    _ensure_capacity(n, &cache, &cap);
    if (!cache) return (seq_t)0;    /* allocation failure */

    /* Initial values for indices 1..9. */
    static const seq_t init[9] = {
        67LL,     146LL,     439LL,
        1187LL,   3227LL,    8964LL,
        24719LL,  67952LL, 187266LL
    };

    if (last == 0) {
        /* First call – populate the first up to 9 values that are needed. */
        idx_t upto = (n < 9) ? n : 9;
        for (idx_t i = 1; i <= upto; ++i) cache[i] = init[i - 1];
        last = upto;
    }

    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i - 1];
        } else {
            /* Recurrence computation; all operations are in seq_t space. */
            cache[i] =  cache[i - 1]
                      + 2 * cache[i - 2]
                      + 6 * cache[i - 3]
                      + 5 * cache[i - 4]
                      +       cache[i - 5]
                      - 2 * cache[i - 6]
                      - 2 * cache[i - 7]
                      -       cache[i - 8];
        }
    }

    last = n;
    return cache[n];
}
