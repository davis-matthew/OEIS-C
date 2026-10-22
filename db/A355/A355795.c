/*
 * A355795  Row 2 of A355793.
 * https://oeis.org/A355795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Computer of OEIS A355795 * * The generating function of the
 *            sequence is * A(x) = ( Σ_{k≥0} t(k+2)/t(2) x^k ) / * ( Σ_{k≥0}
 *            t(k+1)/t(1) x^k ) * where t(n) = ∏_{j=1..n} (3*j-1) (A008544). *
 *            * Let d_k = t(k+1)/t(1) = t(k+1)/2, * n_k = t(k+2)/t(2) =
 *            t(k+2)/10. * Since D(x)=Σ d_k x^k has d_0=1, the co...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifndef __cplusplus
/* C compilation */
static seq_t *_a_cache = NULL;
static idx_t  _a_cache_len = 0;

static seq_t *_t_cache = NULL;
static idx_t  _t_cache_len = 0;

seq_t a(idx_t n)
{
    /* initialise caches */
    if (_a_cache_len == 0) {
        _a_cache = (seq_t *)malloc(sizeof(seq_t));
        _a_cache[0] = (seq_t)1;           /* a(0) = 1 */
        _a_cache_len = 1;
    }
    if (_t_cache_len == 0) {
        _t_cache = (seq_t *)malloc(sizeof(seq_t));
        _t_cache[0] = (seq_t)1;           /* t(0) = 1 */
        _t_cache_len = 1;
    }

    /* extend t_cache up to t(n+2) */
    while (_t_cache_len <= n + 2) {
        idx_t   m       = _t_cache_len;                 /* next index to compute (m) */
        seq_t   last    = _t_cache[_t_cache_len - 1];
        seq_t   mult    = (seq_t)3 * (seq_t)m - (seq_t)1;/* multiplier (3*m-1) */
        seq_t   next    = last * mult;                  /* t(m) = t(m-1)*(3*m-1) */
        seq_t *tmp = (seq_t *)realloc(_t_cache,
                                      sizeof(seq_t) * (_t_cache_len + 1));
        if (!tmp) abort();
        _t_cache = tmp;
        _t_cache[_t_cache_len] = next;
        _t_cache_len++;
    }

    /* compute a values up to n if necessary */
    for (idx_t i = _a_cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t d_k = _t_cache[k + 1] / (seq_t)2;        /* t(k+1)/2   */
            sum += d_k * _a_cache[i - k];
        }
        seq_t n_i  = _t_cache[i + 2] / (seq_t)10;           /* t(i+2)/10 */
        seq_t a_i  = n_i - sum;                            /* recurrence */

        seq_t *tmp2 = (seq_t *)realloc(_a_cache,
                                       sizeof(seq_t) * (_a_cache_len + 1));
        if (!tmp2) abort();
        _a_cache = tmp2;
        _a_cache[_a_cache_len] = a_i;
        _a_cache_len++;
    }

    return _a_cache[n];
}

#else
/* C++ compilation using std::vector for dynamic storage */
#include <vector>

static std::vector<seq_t> _a_cache(1, (seq_t)1); /* a(0)=1 */
static std::vector<seq_t> _t_cache(1, (seq_t)1); /* t(0)=1 */

seq_t a(idx_t n)
{
    /* extend t_cache up to t(n+2) */
    while (_t_cache.size() <= n + 2) {
        idx_t   m    = (idx_t)_t_cache.size();           /* new index */
        seq_t   last = _t_cache.back();
        seq_t   mult = (seq_t)3 * (seq_t)m - (seq_t)1;   /* (3*m-1) */
        _t_cache.push_back(last * mult);
    }

    /* compute a values up to n if necessary */
    while (_a_cache.size() <= n) {
        idx_t i = _a_cache.size();                      /* next n */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t d_k = _t_cache[k + 1] / (seq_t)2;      /* t(k+1)/2   */
            sum += d_k * _a_cache[i - k];
        }
        seq_t n_i = _t_cache[i + 2] / (seq_t)10;         /* t(i+2)/10 */
        _a_cache.push_back(n_i - sum);                  /* recurrence */
    }

    return _a_cache[n];
}

#endif
