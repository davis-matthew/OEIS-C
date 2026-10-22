/*
 * A335392  a(n) is the number of ways to reach n by the process of starting
 *          from 1 and repeatedly adding 5 or multiplying by 3.
 * https://oeis.org/A335392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to reach n from 1 by repeatedly adding 5
 *            or multiplying by 3. Recurrence: a(1) = 1; for n ≥ 2: a(n) = (n
 *            ≥ 6 ? a(n‑5) : 0) + (n % 3 == 0 ? a(n/3) : 0)
 * Range:     u8 n <= 446, u16 n <= 2555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <cstdlib>
#else
#include <stdlib.h>
#endif

/* the build supplies these types */
seq_t a(idx_t n);

/* memoisation cache --------------------------------------------------------*/
#ifdef __cplusplus
static std::vector<seq_t> _cache;   /* indices 0 .. _cap‑1 */
static size_t      _cap   = 0;      /* allocated capacity   */
static idx_t      _known_max = 0;   /* largest n computed   */
#else
static seq_t*     _cache = NULL;   /* malloced array       */
static size_t      _cap   = 0;
static idx_t      _known_max = 0;
#endif

/* ensure capacity for indices up to n */
static void _ensure_capacity(idx_t n)
{
#ifdef __cplusplus
    if ((size_t)n + 1 <= _cap) return;
    size_t new_cap = _cap ? _cap << 1 : (size_t)n + 1;
    if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
    _cache.resize(new_cap);
    _cap = new_cap;
#else
    if (n + 1 <= _cap) return;
    size_t new_cap = _cap ? _cap << 1 : (size_t)n + 1;
    if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
    seq_t* tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    for (idx_t i = _cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    _cache = tmp;
    _cap = new_cap;
#endif
}

/* compute values up to n */
static void _compute_up_to(idx_t n)
{
    if (n <= _known_max) return;
    _ensure_capacity(n);

    if (_known_max == 0) {          /* initialise base case */
#ifdef __cplusplus
        _cache[1] = (seq_t)1;
#else
        _cache[1] = (seq_t)1;
#endif
        _known_max = 1;
    }

    for (idx_t i = _known_max + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        if (i >= 6)        val += _cache[i - 5];
        if (i % 3 == 0)    val += _cache[i / 3];
        _cache[i] = val;
    }
    _known_max = n;
}

/* public function ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    _compute_up_to(n);
    return _cache[n];
}
