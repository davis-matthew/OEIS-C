/*
 * A274856  Number of nX6 0..2 arrays with no element equal to any value at
 *          offset (-1,0) (0,-1) or (-2,-2) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) -77*a(n-2) +286*a(n-3) -736*a(n-4) +1351*a(n-5)
 *            -1787*a(n-6) +1716*a(n-7) -1217*a(n-8) +652*a(n-9) -265*a(n-10)
 *            +78*a(n-11) -14*a(n-12) +a(n-13) Initial values: a(1)=16,
 *            a(2)=243, a(3)=233, a(4)=923, a(5)=3602, a(6)=14414, a(7)=58775,
 *            a(8)=240587, a(9)=983086, a(10)=4014830, a(11...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[16] = {
    0ULL,                        /* index 0 unused */
    16ULL,                       /* a(1) */
    243ULL,                      /* a(2) */
    233ULL,                      /* a(3) */
    923ULL,                      /* a(4) */
    3602ULL,                     /* a(5) */
    14414ULL,                    /* a(6) */
    58775ULL,                    /* a(7) */
    240587ULL,                   /* a(8) */
    983086ULL,                   /* a(9) */
    4014830ULL,                  /* a(10) */
    16413550ULL,                 /* a(11) */
    67168090ULL,                 /* a(12) */
    274928938ULL,                /* a(13) */
    1125151301ULL,               /* a(14) */
    4604230576ULL                /* a(15) */
};

static seq_t * _cache = NULL;   /* array of cached values */
static size_t  _cache_cap = 0;  /* allocated size minus one (index 0 unused) */
static idx_t   _cache_max = 0;  /* highest index that has been computed */

/* Allocate / grow the cache to hold at least n+1 entries (index 0 unused).
   Different allocation strategies are used for C and C++. */
#ifdef __cplusplus
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= _cache_cap) return;
    size_t new_cap = _cache_cap ? _cache_cap << 1 : 64;
    while ((size_t)n + 1 > new_cap) new_cap <<= 1;
    seq_t *tmp = new seq_t[new_cap + 1];
    if (_cache) {
        for (size_t i = 0; i <= _cache_cap; ++i) tmp[i] = _cache[i];
        delete[] _cache;
    }
    _cache = tmp;
    _cache_cap = new_cap;
}
#else
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= _cache_cap) return;
    size_t new_cap = _cache_cap ? _cache_cap << 1 : 64;
    while ((size_t)n + 1 > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    _cache = tmp;
    _cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is the first term. Return 0 for n<1. */
    if (n < 1) return 0;

    /* Cached value exists? */
    if (n <= _cache_max) return _cache[n];

    /* Make room for the requested index. */
    ensure_cache(n);

    /* Initialize the first 15 terms if we have not done so yet. */
    if (_cache_max < 15) {
        idx_t end = 15 < n ? 15 : n;
        for (idx_t i = 1; i <= end; ++i) _cache[i] = _init[i];
        _cache_max = end;
    }

    /* Compute terms beyond the initial block using the recurrence. */
    for (idx_t m = _cache_max + 1; m <= n; ++m) {
        seq_t pos = 0, neg = 0;

        pos += (seq_t)13   * _cache[m-1];
        pos += (seq_t)286  * _cache[m-3];
        pos += (seq_t)1351 * _cache[m-5];
        pos += (seq_t)1716 * _cache[m-7];
        pos += (seq_t)652  * _cache[m-9];
        pos += (seq_t)78   * _cache[m-11];
        pos += (seq_t)1    * _cache[m-13];

        neg += (seq_t)77   * _cache[m-2];
        neg += (seq_t)736  * _cache[m-4];
        neg += (seq_t)1787 * _cache[m-6];
        neg += (seq_t)1217 * _cache[m-8];
        neg += (seq_t)265  * _cache[m-10];
        neg += (seq_t)14   * _cache[m-12];

        /* All terms are non‑negative, so pos >= neg. */
        _cache[m] = pos - neg;
    }

    _cache_max = n;
    return _cache[n];
}
