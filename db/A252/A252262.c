/*
 * A252262  Number of (n+2) X (1+2) 0..3 arrays with every 3 X 3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 2 3 4 6 or 7.
 * https://oeis.org/A252262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + 4*a(n-4) + 4*a(n-5) + 2*a(n-6) for n
 *            > 8 Initial values: a(1)=103, a(2)=130, a(3)=258, a(4)=598,
 *            a(5)=1342, a(6)=2900, a(7)=6330, a(8)=14024
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 24, u64 n <= 52, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t _init[9] = {0ULL,
    103ULL, 130ULL, 258ULL, 598ULL,
    1342ULL, 2900ULL, 6330ULL, 14024ULL};

static seq_t * _cache      = NULL;   /* raw array, 1‑based indexing */
static size_t  _cache_size = 0;      /* number of usable indices (size) */
static idx_t   _last_valid = 0;      /* largest n for which _cache[n] is valid */

/* Ensure _cache has room for indices 1..n (i.e. size n+1). */
static void _ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= _cache_size)
        return;

    size_t new_sz = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_sz * sizeof(seq_t));
    if (!new_cache)
        abort();

    /* copy current valid values */
    if (_cache)
        for (idx_t i = 1; i <= _last_valid; ++i)
            new_cache[i] = _cache[i];

    /* zero the rest */
    for (size_t i = _last_valid + 1; i <= new_sz - 1; ++i)
        new_cache[i] = 0;

    /* initialise the first eight terms (overlap is harmless) */
    for (idx_t i = 1; i <= 8 && i <= n; ++i)
        new_cache[i] = _init[i];

    free(_cache);
    _cache      = new_cache;
    _cache_size = new_sz;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but return 0 safely */

    if (n <= 8)
        return _init[n];

    _ensure_cache(n);

    /* compute missing terms up to n */
    idx_t start = _last_valid + 1;
    if (start < 9) start = 9;         /* recurrence valid only for n>8 */

    for (idx_t k = start; k <= n; ++k) {
        _cache[k] = _cache[k-1]
                  + 3 * _cache[k-3]
                  + 4 * _cache[k-4]
                  + 4 * _cache[k-5]
                  + 2 * _cache[k-6];
    }
    _last_valid = n;
    return _cache[n];
}
