/*
 * A373759  Expansion of e.g.f. exp(x^4/(24 * (1 - x)^3)).
 * https://oeis.org/A373759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} binomial(n-k-1, n-4k)/(24^k *
 *            k!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(n) = ((n-1)!/24) * Sum_{k=4..n} [k * binomial(k-2, k-4) * a(n-k)/(n-k)!] */
/* with binomial(k-2,k-4) = ((k-2)(k-3))/2. */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations of external helper */
extern seq_t binomial(idx_t a, idx_t b);
extern seq_t pow_uint(uint64_t base, idx_t exp);

/* Cached sequence values: cache[i] = a(i) for i <= cache_cap-1 */
static seq_t *cache  = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 0;      /* highest index that has been computed */
static bool cache_initialized = false;

/* Ensure cache has space for indices up to `needed`. */
static void ensure_cache(idx_t needed)
{
    if ((size_t)needed < cache_cap)
        return;

    size_t new_cap = (cache_cap ? cache_cap * 2 : 1);
    while (new_cap <= (size_t)needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise cache for the zero element once. */
    if (!cache_initialized) {
        ensure_cache(0);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_initialized = true;
        cache_last = 0;
    }

    if (n == 0)
        return cache[0];
    if (n <= 3) {
        /* Ensure cache[1..3] are defined. */
        if (cache_last < 3) {
            ensure_cache(3);
            for (idx_t i = cache_last + 1; i <= 3; ++i)
                cache[i] = (seq_t)0;
            cache_last = 3;
        }
        return cache[n];
    }

    /* Compute missing terms up to n. */
    if (cache_last < n) {
        ensure_cache(n);
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            if (i <= 3) {
                cache[i] = (seq_t)0;
                continue;
            }
            /* Compute a(i) using the recurrence. */
            seq_t sum = (seq_t)0;

            /* prod = (i-3)*(i-2)*(i-1) for k = 4 */
            seq_t prod = (seq_t)(i - 3);
            prod *= (seq_t)(i - 2);
            prod *= (seq_t)(i - 1);

            for (idx_t k = 4; k <= i; ++k) {
                /* c = k*(k-2)*(k-3)/2 */
                seq_t c = (seq_t)k;
                c *= (seq_t)(k - 2);
                c *= (seq_t)(k - 3);
                c /= (seq_t)2;

                seq_t term = cache[i - k];   /* a(i-k) */
                term *= c;                   /* * c */
                term *= prod;                /* * prod */
                term /= (seq_t)24;           /* / 24  (exact division) */
                sum += term;

                if (k < i) {
                    /* Update prod to (i-(k+1)+1 .. i-1) => multiply by (i-k) */
                    prod *= (seq_t)(i - k);
                }
            }
            cache[i] = sum;
        }
        cache_last = n;
    }

    return cache[n];
}
