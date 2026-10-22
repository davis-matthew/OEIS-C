/*
 * A369403  Expansion of (1/x) * Series_Reversion( x / (1+x)^3 * (1-x^3)^3 ).
 * https://oeis.org/A369403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(3*n+k+2,k) *
 *            binomial(3*n+3,n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A369403 sequence.  It caches results so that
 * calls may be made in any order while remaining fast for n up
 * to about 10 000.  All arithmetic is performed in the unsigned
 * type seq_t, which may be 8,16,32,64‑bit or arbitrary precision.
 */

#include <stdlib.h>
#include <stdint.h>                 /* for uint64_t if needed */
#include "../utils/binomial.h"     /* seq_t binomial(idx_t, idx_t) */
#include "../utils/seq_realloc.h"  /* seq_t *seq_realloc(seq_t *, size_t, size_t) */

static seq_t *cache      = NULL;  /* cache for computed terms */
static idx_t cache_size  = 0;     /* number of terms currently cached (size of cache) */

/* Ensure that terms 0 .. n are computed and stored in cache. */
static void ensure_cached(idx_t n)
{
    if (n + 1 <= cache_size) {
        return;                       /* everything we need is already cached */
    }

    /* Resize the cache to hold terms 0 .. n */
    size_t old_size = cache_size;
    seq_t *tmp = seq_realloc(cache, old_size, (size_t)(n + 1));
    if (tmp == NULL) {
        abort();                       /* allocation failed */
    }
    cache = tmp;
    cache_size = (size_t)(n + 1);

    /* Compute any newly required terms */
    for (idx_t i = old_size; i <= n; ++i) {
        idx_t kmax = i / 3;
        seq_t sum = 0;
        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t term1 = binomial(3 * i + k + 2, k);
            seq_t term2 = binomial(3 * i + 3, i - 3 * k);
            sum += term1 * term2;
        }
        cache[i] = sum / (((seq_t)i) + 1);
    }
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cached(n);
    return cache[n];
}
