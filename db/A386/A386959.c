/*
 * A386959  a(n) = Sum_{k=0..n} 8^k * binomial(k-2/3,k) *
 *          binomial(2*n-2/3,n-k).
 * https://oeis.org/A386959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 8^k * binomial(k-2/3,k) *
 *            binomial(2*n-2/3,n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence (valid for n ≥ 3):
        32*n*(n-1)*a(n) -4*(n-1)*(215*n-376)*a(n-1)
        +3*(2353*n^2-9810*n+9920)*a(n-2)
        -918*(3*n-7)*(6*n-17)*a(n-3) = 0
      a(0)=1, a(1)=4, a(2)=27
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t * _cache = NULL;     /* memoisation array */
static size_t   _cache_cap = 0;   /* number of allocated entries */

/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* resize cache if needed */
    if ((idx_t)_cache_cap <= n) {
        size_t old_cap = _cache_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) {          /* allocation failure – return zero */
            _cache_cap = 0;
            _cache = NULL;
            return (seq_t)0;
        }
        _cache = tmp;
        _cache_cap = new_cap;

        /* compute new entries starting from old_cap up to n */
        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            idx_t m = (idx_t)i;
            if (m == 0) {
                _cache[i] = (seq_t)1;
            } else if (m == 1) {
                _cache[i] = (seq_t)4;
            } else if (m == 2) {
                _cache[i] = (seq_t)27;
            } else {
#ifdef __cplusplus
                /* use big‑integer arithmetic (operator overloading handles size) */
                seq_t term1 = (seq_t)4 * (seq_t)(m-1) * (seq_t)(215*m - 376)
                               * _cache[i-1];
                seq_t term2 = (seq_t)3 * (seq_t)(2353*m*m - 9810*m + 9920)
                               * _cache[i-2];
                seq_t term3 = (seq_t)918 * (seq_t)(3*m-7)
                               * (seq_t)(6*m-17) * _cache[i-3];
                seq_t numer = term1 - term2 + term3;
                seq_t denom = (seq_t)32 * (seq_t)m * (seq_t)(m-1);
                _cache[i] = numer / denom;
#else
                /* use 128‑bit arithmetic for builtin integer types */
                __int128 mm = m;
                __int128 term1 = (__int128)4 * (mm-1) *
                                 ((__int128)215*mm - 376) *
                                 (__int128)_cache[i-1];
                __int128 term2 = (__int128)3 *
                                 ((__int128)2353*mm*mm - 9810*mm + 9920) *
                                 (__int128)_cache[i-2];
                __int128 term3 = (__int128)918 *
                                 ((__int128)3*mm - 7) *
                                 ((__int128)6*mm - 17) *
                                 (__int128)_cache[i-3];
                __int128 numer = term1 - term2 + term3;
                __int128 denom = (__int128)32 * mm * (mm-1);
                _cache[i] = (seq_t)(numer / denom);
#endif
            }
        }
    }

    return _cache[n];
}
