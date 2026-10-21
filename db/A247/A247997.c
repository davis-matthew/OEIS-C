/*
 * A247997  Number of length 2+5 0..n arrays with no disjoint triples in any
 *          consecutive six terms having the same sum.
 * https://oeis.org/A247997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 4*a(n-3) - 3*a(n-4) + 7*a(n-6) +
 *            4*a(n-7) - 5*a(n-8) - 4*a(n-9) - 5*a(n-10) + 4*a(n-11) +
 *            7*a(n-12) - 3*a(n-14) - 4*a(n-15) + 2*a(n-16) + 2*a(n-17) -
 *            a(n-18) Initial values (a(0)..a(18)): a(0) = 0, a(1)=32,
 *            a(2)=702, a(3)=5316, a(4)=27800, a(5)=104620, a(6)=329742, a(...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 24, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* Cache for previously computed values.  In C++ we use std::vector,
   in C we use a malloc’ed array.  The cache always contains a(0)
   at index 0. */
#ifdef __cplusplus
static std::vector<seq_t> _cache;
#else
static seq_t * _cache = NULL;
static size_t   _cache_size = 0;    /* number of allocated elements  */
#endif

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS offset; no need to handle n==0 */
    if (n < 1) return 0;

    /*--- initialise the cache with the first 19 terms ---*/
#ifdef __cplusplus
    if (_cache.empty())
    {
        _cache.resize(19);            /* indices 0..18 */
        _cache[0]  = (seq_t)0;
        _cache[1]  = (seq_t)32;
        _cache[2]  = (seq_t)702;
        _cache[3]  = (seq_t)5316;
        _cache[4]  = (seq_t)27800;
        _cache[5]  = (seq_t)104620;
        _cache[6]  = (seq_t)329742;
        _cache[7]  = (seq_t)884032;
        _cache[8]  = (seq_t)2131356;
        _cache[9]  = (seq_t)4664480;
        _cache[10] = (seq_t)9508130;
        _cache[11] = (seq_t)18168932;
        _cache[12] = (seq_t)33008212;
        _cache[13] = (seq_t)57264516;
        _cache[14] = (seq_t)95672090;
        _cache[15] = (seq_t)154419880;
        _cache[16] = (seq_t)242095992;
        _cache[17] = (seq_t)369529512;
        _cache[18] = (seq_t)551174206;
    }
#else
    if (_cache_size == 0)
    {
        size_t sz = 19;
        /* malloc returns void*, cast to seq_t* is allowed in C */
        _cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!_cache) exit(1);
        _cache_size = sz;

        _cache[0]  = (seq_t)0;
        _cache[1]  = (seq_t)32;
        _cache[2]  = (seq_t)702;
        _cache[3]  = (seq_t)5316;
        _cache[4]  = (seq_t)27800;
        _cache[5]  = (seq_t)104620;
        _cache[6]  = (seq_t)329742;
        _cache[7]  = (seq_t)884032;
        _cache[8]  = (seq_t)2131356;
        _cache[9]  = (seq_t)4664480;
        _cache[10] = (seq_t)9508130;
        _cache[11] = (seq_t)18168932;
        _cache[12] = (seq_t)33008212;
        _cache[13] = (seq_t)57264516;
        _cache[14] = (seq_t)95672090;
        _cache[15] = (seq_t)154419880;
        _cache[16] = (seq_t)242095992;
        _cache[17] = (seq_t)369529512;
        _cache[18] = (seq_t)551174206;
    }
#endif

    /*--- ensure the cache reaches index n ---*/
#ifdef __cplusplus
    if (_cache.size() <= (size_t)n)
    {
        _cache.resize(n + 1);
        for (size_t i = 19; i <= (size_t)n; ++i)
        {
            seq_t res = 0;
            res += _cache[i-1] * 2;
            res += _cache[i-2] * 2;
            res += _cache[i-6] * 7;
            res += _cache[i-7] * 4;
            res += _cache[i-11] * 4;
            res += _cache[i-12] * 7;
            res += _cache[i-16] * 2;
            res += _cache[i-17] * 2;

            res -= _cache[i-3]  * 4;
            res -= _cache[i-4]  * 3;
            res -= _cache[i-8]  * 5;
            res -= _cache[i-9]  * 4;
            res -= _cache[i-10] * 5;
            res -= _cache[i-14] * 3;
            res -= _cache[i-15] * 4;
            res -= _cache[i-18];
            _cache[i] = res;
        }
    }
    return _cache[(size_t)n];
#else
    if (_cache_size <= (size_t)n)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        _cache = tmp;
        _cache_size = new_size;
        for (size_t i = 19; i <= (size_t)n; ++i)
        {
            seq_t res = 0;
            res += _cache[i-1] * 2;
            res += _cache[i-2] * 2;
            res += _cache[i-6] * 7;
            res += _cache[i-7] * 4;
            res += _cache[i-11] * 4;
            res += _cache[i-12] * 7;
            res += _cache[i-16] * 2;
            res += _cache[i-17] * 2;

            res -= _cache[i-3]  * 4;
            res -= _cache[i-4]  * 3;
            res -= _cache[i-8]  * 5;
            res -= _cache[i-9]  * 4;
            res -= _cache[i-10] * 5;
            res -= _cache[i-14] * 3;
            res -= _cache[i-15] * 4;
            res -= _cache[i-18];
            _cache[i] = res;
        }
    }
    return _cache[(size_t)n];
#endif
}
