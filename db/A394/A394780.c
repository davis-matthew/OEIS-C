/*
 * A394780  G.f. A(x) satisfies A(x) = 1 - x/2 * d/dx log(1 - 2*x*A(x)^2).
 * https://oeis.org/A394780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A394780
    Recurrence:
        a(0) = 1
        a(1) = 1
        a(n) = (n-2) * Σ_{k=0}^{n-1} a(k) * a(n-1-k)
               + 2 * Σ_{i+j+k=n-1} a(i) * a(j) * a(k)
    Implementation uses convolution arrays to compute the sums in O(n²).
*/

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* Global cache for computed sequence terms and convolution with itself */
static seq_t *cache      = NULL;   /* a[0..cache_len-1]           */
static size_t cache_len  = 0;       /* number of terms stored      */
static seq_t *b_cache    = NULL;   /* b[n] = Σ_{i=0}^n a[i] * a[n-i]  */
static size_t b_len      = 0;       /* allocated size of b_cache   */

/* Ensure that cache and b_cache are large enough for terms up to n */
static void ensure_cache(idx_t n)
{
    /* Initialisation of a(0) and b(0) */
    if (cache_len == 0) {
        size_t init_cap = 1;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_len = 1;

        b_len = 1;
        b_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!b_cache) abort();
        b_cache[0] = cache[0] * cache[0]; /* b[0] = a0*a0 = 1 */
    }

    if (n < cache_len)
        return; /* already computed */

    /* Expand cache if necessary */
    if (cache_len <= n) {
        size_t new_cap = cache_len ? cache_len * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache && new_cap > 0) abort();
        cache = new_cache;
        memset(&cache[cache_len], 0, (new_cap - cache_len) * sizeof(seq_t));
    }

    /* Main loop: compute terms from current cache_len up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val;
        if (k == 0) {
            val = (seq_t)1;              /* a(0) already set */
        } else if (k == 1) {
            val = (seq_t)1;              /* a(1) = 1 */
        } else {
            /* b[k-1] is convolution of a with itself up to index k-1 */
            seq_t b_nm1 = b_cache[k - 1];

            /* compute c[k-1] = Σ_{i=0}^{k-1} a[i] * b[k-1-i] */
            seq_t c_nm1 = 0;
            for (idx_t i = 0; i < k; ++i)
                c_nm1 += cache[i] * b_cache[(k - 1) - i];

            /* a(k) = (k-2)*b[k-1] + 2*c[k-1] */
            seq_t coeff = (seq_t)(k - 2);
            val = coeff * b_nm1 + (seq_t)2 * c_nm1;
        }
        cache[k] = val;

        /* Update b_cache for new contributions involving a[k] */
        /* Ensure b_cache can store indices up to 2*k */
        size_t needed_b = 2 * k + 1;
        if (b_len < needed_b) {
            size_t new_b = b_len ? b_len * 2 : 4;
            if (new_b < needed_b) new_b = needed_b;
            seq_t *tmp = (seq_t *)realloc(b_cache, new_b * sizeof(seq_t));
            if (!tmp) abort();
            memset(tmp + b_len, 0, (new_b - b_len) * sizeof(seq_t));
            b_cache = tmp;
            b_len = new_b;
        }

        /* Add contributions of a[k] to b_cache */
        for (idx_t i = 0; i < k; ++i) {
            size_t idx = k + i;
            b_cache[idx] += val * cache[i] * (seq_t)2;
        }
        b_cache[2 * k] += val * val;
    }

    cache_len = (size_t)n + 1;
}

/* Public sequence function required by the framework */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
