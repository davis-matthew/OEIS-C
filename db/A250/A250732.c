/*
 * A250732  Number of (3+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 9*a(n-3) + 12*a(n-4) - 2*a(n-5) -
 *            3*a(n-6) + a(n-7) for n > 8 Initial values: a(1)=33, a(2)=85,
 *            a(3)=208, a(4)=496, a(5)=1158, a(6)=2699, a(7)=6257, a(8)=14520
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 49, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#endif
#include <stdlib.h>   /* malloc, realloc, free */

static const seq_t _init8[8] = {
    (seq_t)33, (seq_t)85, (seq_t)208, (seq_t)496,
    (seq_t)1158, (seq_t)2699, (seq_t)6257, (seq_t)14520
};

#if defined(__cplusplus)
/* C++ implementation: use std::vector for automatic construction/destruction */
static std::vector<seq_t> _vec;   /* stores a(1), a(2), … */

static void _ensure_largest(idx_t n)
{
    if (_vec.empty())
    {
        /* initialise with the 8 known values */
        _vec.reserve(16);
        for (int i = 0; i < 8; ++i) _vec.push_back(_init8[i]);
    }

    /* compute missing terms in increasing order */
    while (_vec.size() < (size_t)n)
    {
        size_t pos = _vec.size() + 1;      /* 1‑based index of new term */
        seq_t val = 4 * _vec[pos - 2];
        val -= 2 * _vec[pos - 3];
        val -= 9 * _vec[pos - 4];
        val += 12 * _vec[pos - 5];
        val -= 2 * _vec[pos - 6];
        val -= 3 * _vec[pos - 7];
        val += _vec[pos - 8];
        _vec.push_back(val);
    }
}
#else
/* C implementation: use realloc; seq_t is a POD integer type */
static seq_t * _cache = NULL;     /* 0‑based array of a(1), a(2), … */
static size_t _cache_len = 0;     /* number of stored terms   */
static size_t _cache_cap = 0;     /* allocated capacity       */

static void _grow_cache(size_t need)
{
    if (_cache_cap >= need) return;
    size_t new_cap = _cache_cap ? _cache_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* abort if memory allocation fails */
    _cache = tmp;
    _cache_cap = new_cap;
}

static void _ensure_largest(idx_t n)
{
    if (_cache == NULL)
    {
        _grow_cache(16);
        /* initialise with the 8 known values */
        for (size_t i = 0; i < 8; ++i) _cache[i] = _init8[i];
        _cache_len = 8;
    }

    /* compute missing terms in order */
    while (_cache_len < (size_t)n)
    {
        _grow_cache(_cache_len + 1);
        seq_t val = 4 * _cache[_cache_len - 1];
        val -= 2 * _cache[_cache_len - 2];
        val -= 9 * _cache[_cache_len - 3];
        val += 12 * _cache[_cache_len - 4];
        val -= 2 * _cache[_cache_len - 5];
        val -= 3 * _cache[_cache_len - 6];
        val += _cache[_cache_len - 7];
        _cache[_cache_len] = val;
        ++_cache_len;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 as a placeholder */
    _ensure_largest(n);
#if defined(__cplusplus)
    return _vec[(size_t)n - 1];
#else
    return _cache[(size_t)n - 1];
#endif
}
