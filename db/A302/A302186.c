/*
 * A302186  Number of 3D walks of type ace.
 * https://oeis.org/A302186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) satisfies the recurrence
 *            (n+3)(n+2)a(n)+4(-3n^2-8n-3)a(n-1)+8(4n^2+3n-3)a(n-2)
 *            +48(n-2)^2a(n-3)+144(-(n-2)(n-3))a(n-4)=0, with initial values
 *            a(0)=1, a(1)=3, a(2)=11, a(3)=44
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

#if defined(__cplusplus)
/* C++ implementation ------------------------------------------------------- */
static inline seq_t compute_next(idx_t n, seq_t a0, seq_t a1, seq_t a2, seq_t a3)
{
    seq_t nn = n;

    seq_t coeff1 = (seq_t)12 * nn * nn + (seq_t)32 * nn + (seq_t)12;
    seq_t term1  = coeff1 * a3;

    seq_t coeff2 = (seq_t)8 * ((seq_t)4 * nn * nn + (seq_t)3 * nn - (seq_t)3);
    seq_t term2  = coeff2 * a2;

    seq_t coeff3 = (seq_t)48 * (nn - 2) * (nn - 2);
    seq_t term3  = coeff3 * a1;

    seq_t coeff4 = (seq_t)144 * (nn - 2) * (nn - 3);
    seq_t term4  = coeff4 * a0;

    seq_t numer = term1 - term2 - term3 + term4;
    seq_t denom = (nn + 3) * (nn + 2);

    return numer / denom;
}
#else
/* C implementation -------------------------------------------------------- */
static inline seq_t compute_next(idx_t n, seq_t a0, seq_t a1, seq_t a2, seq_t a3)
{
    unsigned __int128 nn   = n;

    unsigned __int128 coeff1 = (unsigned __int128)12 * nn * nn
                           + (unsigned __int128)32 * nn
                           + (unsigned __int128)12;
    unsigned __int128 term1  = coeff1 * (unsigned __int128)a3;

    unsigned __int128 coeff2 = (unsigned __int128)8 * (
                (unsigned __int128)4 * nn * nn
              + (unsigned __int128)3 * nn
              - (unsigned __int128)3 );
    unsigned __int128 term2  = coeff2 * (unsigned __int128)a2;

    unsigned __int128 coeff3 = (unsigned __int128)48 * (nn - 2) * (nn - 2);
    unsigned __int128 term3  = coeff3 * (unsigned __int128)a1;

    unsigned __int128 coeff4 = (unsigned __int128)144 * (nn - 2) * (nn - 3);
    unsigned __int128 term4  = coeff4 * (unsigned __int128)a0;

    unsigned __int128 numer = term1 - term2 - term3 + term4;
    unsigned __int128 denom = (nn + 3) * (nn + 2);

    return (seq_t)(numer / denom);
}
#endif

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;          /* number of entries currently stored */

    /* ensure we have a storage area for at least n+1 values */
    if (cache == NULL || (size_t)n + 1 > cache_cap)
        ensure_cache((idx_t)(n + 1) - 1, &cache, &cache_cap);

    /* initialise the first four terms once */
    if (cache_len == 0) {
        /* make sure we have room for the first four entries */
        if (cache_cap < 4)
            ensure_cache(3, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)44;
        cache_len = 4;              /* we now have indices 0..3 stored */
    }

    if (n < cache_len)
        return cache[n];

    /* compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a0 = cache[(size_t)(i - 4)];
        seq_t a1 = cache[(size_t)(i - 3)];
        seq_t a2 = cache[(size_t)(i - 2)];
        seq_t a3 = cache[(size_t)(i - 1)];
        cache[(size_t)i] = compute_next(i, a0, a1, a2, a3);
    }
    cache_len = n + 1;
    return cache[n];
}
