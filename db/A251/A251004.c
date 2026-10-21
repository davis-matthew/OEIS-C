/*
 * A251004  Number of (n+1)X(2+1) 0..2 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than the absolute difference of its
 *          antidiagonal elements.
 * https://oeis.org/A251004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) -169*a(n-2) +800*a(n-3)
 *            -2847*a(n-4)+5874*a(n-5) -6458*a(n-6)
 *            +2876*a(n-7)+558*a(n-8)-1473*a(n-9)+1896*a(n-10)
 *            -1394*a(n-11)+460*a(n-12)-68*a(n-13); Initial terms: a(1)=530,
 *            a(2)=10743, a(3)=218298, a(4)=4434257, a(5)=90065275,
 *            a(6)=1829373498, a(7)=37157718881, a(8)=754736624943...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple realloc wrapper for the sequence cache */
static seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap)
{
    (void)old_cap;                /* unused, realloc handles size internally */
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
}

/* Computes the n-th term of A251004 (1‑based offset) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* Undefined for n=0, return 0 */

    /* Convert index to a size_t for array indexing (safe for n≤10 000) */
    size_t idx = (size_t)n;

    /* Dynamic cache of terms already computed */
    static seq_t *cache = NULL;
    static size_t cap   = 0;             /* currently allocated capacity */

    /* Ensure sufficient capacity */
    if (idx >= cap) {
        size_t new_cap = (cap == 0) ? 16 : (idx + 16);
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp)             /* allocation failure – return 0 as fallback */
            return 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Ensure initial 13 terms are stored in the cache exactly once */
    static bool init_done = false;
    if (!init_done) {
        /* Initial values (copied into cache) */
        static const seq_t init[13] = {
            530ULL,
            10743ULL,
            218298ULL,
            4434257ULL,
            90065275ULL,
            1829373498ULL,
            37157718881ULL,
            754736624943ULL,
            15329985552866ULL,
            311378109150967ULL,
            6324619621199857ULL,
            128463794143755317ULL,
            2609318408138865875ULL
        };
        for (size_t i = 1; i <= 13 && i < cap; ++i)
            cache[i] = init[i-1];
        init_done = true;
    }

    /* Return if term is already computed */
    if (idx <= 13)
        return cache[idx];

    /* Recurrence coefficients (13 terms) */
    static const int coeff[13] = {
        27,   -169,   800,   -2847,  5874,
        -6458, 2876,   558,   -1473,  1896,
        -1394, 460,   -68
    };

    /* Compute missing terms up to idx */
    for (size_t k = 14; k <= idx; ++k) {
        seq_t val = 0;
        for (int i = 0; i < 13; ++i) {
            int ci = coeff[i];
            seq_t prod = cache[k-1-i] * (seq_t)(ci > 0 ? ci : -ci);
            if (ci > 0)
                val += prod;
            else
                val -= prod;   /* result remains non‑negative for the given sequence */
        }
        cache[k] = val;
    }

    return cache[idx];
}
