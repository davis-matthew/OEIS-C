/*
 * A382372  Expansion of 1/( 1 - Sum_{k>=0} x^(4^k) / (1 - x^(4^k)) ).
 * https://oeis.org/A382372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>=1 a(n) = Σ_{k=1..n} (⌊ν₂(k)/2⌋ + 1) * a(n‑k)
 *            where ν₂(k) is the exponent of 2 in k (equivalently, a(n) =
 *            Σ_{k=1..n} w(k) * a(n‑k) with w(k)=v4(k)+1, v4(k)=max e:4^e|k
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   v4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/v4.h"

/* -------------------------------------- */
/*  Memoized implementation up to n = 10000  */
/*  (enough for all expected test cases)   */
/* -------------------------------------- */
static seq_t cache[10001];           /* cache[0] .. cache[10000] */
static int  cache_len = 0;           /* number of terms already filled (>=1) */

/* ---------------------------------------------------------- */
/*   Public function required by the build                    */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n >= sizeof(cache)/sizeof(cache[0]))
    {
        /* n too large for the fixed size cache – fall back to on‑the‑fly
           computation (unlikely for the test data). */
        seq_t result = 1;                /* a(0) */
        if (n == 0) return result;
        /* dynamic reusable buffer (small enough) */
        seq_t *buf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!buf) return 0;              /* allocation failure – return zero */
        buf[0] = result;
        for (idx_t i = 1; i <= n; ++i)
        {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += ((seq_t)(v4(k) + 1)) * buf[i - k];
            buf[i] = sum;
        }
        result = buf[n];
        free(buf);
        return result;
    }

    /* initialise the cache lazily */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* extend cache up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += ((seq_t)(v4(k) + 1)) * cache[i - k];
        cache[i] = sum;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
