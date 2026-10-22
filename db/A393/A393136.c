/*
 * A393136  G.f. A(x) satisfies A(x) = 1 - E(log(1 - x*(1 + E(A(x))))), where
 *          E is the Euler operator x*d/dx.
 * https://oeis.org/A393136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393136
   Recurrence: a(0)=1,
   a(n) = ((n-1)^2+1)*a(n-1) + Σ_{k=1}^{n-1} k*a(k)*a(n-1-k)   for n≥1
   (indices as in OEIS: a(0) is the first term)
*/

#include <stdlib.h>

static void ensure_capacity(idx_t req)
{
    /* Expand the static cache array (exclusive of 0-based index) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last_idx = 0;        /* largest n fully computed */

    /* Store back-pointer to the outer function's static variables */
    if (req > (idx_t)(cap - 1)) {
        size_t new_cap = (size_t)(req + 1);
        if (new_cap < 4)
            new_cap = 4;
        while (new_cap <= (size_t)req)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* Initialize newly allocated slots to zero */
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }
    /* Return the internally stored pointers via external linkage by
       placing them in static variables inside the outer function. */
    /* Effectively, this function is a thin wrapper: it guarantees
       that the cache array is large enough. */
    (void)0;
}

static void* _cache_ptr = NULL;   /* pointer to the cache array */
static size_t _cap = 0;           /* capacity of the cache array */
static idx_t _last_idx = 0;       /* largest n fully computed */

static void ensure_cache(idx_t n)
{
    /* Wrapper to guarantee cache exists and has at least n+1 slots
       and to return its pointer and capacity via static variables. */
    if (_cache_ptr == NULL) {
        _cache_ptr = malloc(sizeof(seq_t) * 4);
        if (!_cache_ptr) abort();
        _cap = 4;
        ((seq_t *)_cache_ptr)[0] = (seq_t)1;    /* a(0) = 1 */
        for (size_t i = 1; i < _cap; ++i)
            ((seq_t *)_cache_ptr)[i] = (seq_t)0;
    }
    if (n >= (idx_t)_cap) {
        size_t new_cap = _cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_cache_ptr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        _cache_ptr = tmp;
        for (size_t i = _cap; i < new_cap; ++i)
            ((seq_t *)_cache_ptr)[i] = (seq_t)0;
        _cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    seq_t *cache = (seq_t *)_cache_ptr;

    /* If this is the first call, a(0) is already set (see ensure_cache) */
    if (n == 0)
        return cache[0];

    /* Compute values from last computed + 1 up to n */
    for (idx_t i = _last_idx + 1; i <= n; ++i) {
        idx_t im1 = i - 1;
        seq_t prev = cache[im1];
        seq_t term1 = (((seq_t)im1 * (seq_t)im1) + (seq_t)1) * prev;

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            sum += (seq_t)k * cache[k] * cache[im1 - k];
        }
        cache[i] = term1 + sum;
    }
    _last_idx = n;
    return cache[n];
}
