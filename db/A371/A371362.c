/*
 * A371362  Expansion of (1/x) * Series_Reversion( x * (1-3*x)^2 / (1-2*x) ).
 * https://oeis.org/A371362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(n) = (1/(n+1)) * Sum_{k=0..n} 2^(n-k) *
   binomial(2n+k+1,k) * binomial(2n,n-k) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* This implementation uses 128‑bit intermediates to avoid overflow
 * for the 64‑bit build.  On builds where seq_t is a larger integer
 * type or a big‑integer class, the compiler will still accept the
 * casts because the 128‑bit values are implicitly convertible to
 * seq_t.  The algorithm is memoised: values once computed are stored
 * in a static array so that subsequent calls (in any order) use
 * the cached results. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n < cache_len)                 /* already computed */
        return cache[n];

    /* grow cache */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)                       /* allocation failed */
            return (seq_t)0;
        cache = tmp;

        /* compute missing entries */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            __int128 sum   = 0;
            __int128 pow2  = 1;            /* 2^i */
            for (idx_t j = 0; j < i; ++j)
                pow2 <<= 1;

            __int128 Ak    = 1;                     /* binom(2i+1,0) */
            __int128 Bk    = ( __int128 )binomial(2 * i, i); /* binom(2i,i) */

            for (idx_t k = 0; k <= i; ++k) {
                /* term = 2^(i-k) * Ak * Bk */
                __int128 term = pow2 * Ak * Bk;
                sum += term;

                if (k == i)
                    break;

                /* update Ak: Ak = binom(2i+k+2, k+1) */
                __int128 numAk = ( __int128 )(2 * i + k + 2);
                __int128 denAk = ( __int128 )(k + 1);
                Ak = (Ak * numAk) / denAk;

                /* update Bk: Bk = binom(2i, i-(k+1)) */
                __int128 numBk = ( __int128 )(i - k);
                __int128 denBk = ( __int128 )(i + k + 1);
                Bk = (Bk * numBk) / denBk;

                /* power of two for next k */
                pow2 >>= 1;
            }

            /* divide by (i+1) to finish the formula */
            cache[i] = (seq_t)(sum / ( (__int128)(i + 1) ));
        }
        cache_len = needed;
    }

    return cache[n];
}
