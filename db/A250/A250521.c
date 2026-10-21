/*
 * A250521  Number of (n+1)X(2+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 14*a(n-1) - 85*a(n-2) + 294*a(n-3) - 639*a(n-4) +
 *            906*a(n-5) * - 839*a(n-6) + 490*a(n-7) - 164*a(n-8) + 24*a(n-9)
 *            * * Initial values: * a(1)=222, a(2)=1180, a(3)=5029,
 *            a(4)=18859, a(5)=65310, * a(6)=214812, a(7)=682921,
 *            a(8)=2122743, a(9)=6501118
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static const seq_t _init_seq[9] = {
    (seq_t)222ULL,
    (seq_t)1180ULL,
    (seq_t)5029ULL,
    (seq_t)18859ULL,
    (seq_t)65310ULL,
    (seq_t)214812ULL,
    (seq_t)682921ULL,
    (seq_t)2122743ULL,
    (seq_t)6501118ULL
};

static seq_t * _cache = NULL;
static idx_t   _cache_cap = 0;
static idx_t   _cache_len = 0;

/* Expand the cache so that indices up to n are available. */
static void _ensure_cache(idx_t n)
{
    if (n <= _cache_cap) return;
    idx_t new_cap = _cache_cap ? _cache_cap << 1 : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_cache, sizeof(seq_t) * new_cap);
    if (!tmp) abort();
    _cache = tmp;
    _cache_cap = new_cap;
}

/* Initialise the first nine terms in the cache. */
static void _init_cache(void)
{
    _ensure_cache(10);            /* we need space for indices 0..9 */
    _cache_len = 9;
    for (idx_t i = 1; i <= 9; ++i)
        _cache[i] = _init_seq[i-1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 9) return _init_seq[n-1];

    if (_cache_len == 0) _init_cache();

    if (n <= _cache_len) return _cache[n];

    _ensure_cache(n + 1);         /* space for index n */

    for (idx_t i = _cache_len + 1; i <= n; ++i)
    {
        /* compute a(i) using only non‑negative additions and a final subtraction
         * that is guaranteed non‑negative because a(i) itself is non‑negative.   */
        seq_t sum1 = _cache[i-1] * (seq_t)14
                   + _cache[i-3] * (seq_t)294
                   + _cache[i-5] * (seq_t)906
                   + _cache[i-7] * (seq_t)490
                   + _cache[i-9] * (seq_t)24;
        seq_t sum2 = _cache[i-2] * (seq_t)85
                   + _cache[i-4] * (seq_t)639
                   + _cache[i-6] * (seq_t)839
                   + _cache[i-8] * (seq_t)164;
        _cache[i] = sum1 - sum2;
    }
    _cache_len = n;
    return _cache[n];
}
