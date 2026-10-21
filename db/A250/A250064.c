/*
 * A250064  Number of length 4+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) +
 *            462*a(n-5) - 462*a(n-6) + 330*a(n-7) - 165*a(n-8) + 55*a(n-9) -
 *            11*a(n-10) + a(n-11), with initial values a(1)..a(11) computed
 *            via the empirical polynomial
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 84, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


static seq_t compute_base(idx_t n) {
    seq_t s  = (seq_t)n;
    seq_t n2 = s * s;
    seq_t n3 = n2 * s;
    seq_t n4 = n3 * s;
    seq_t n5 = n4 * s;
    seq_t n6 = n5 * s;
    seq_t n7 = n6 * s;
    seq_t n8 = n7 * s;
    seq_t n9 = n8 * s;
    seq_t n10 = n9 * s;

    /* numerator = 42*n10 + 42*n9 + 240*n8 + 294*n7 + 280*n5 + 175*n4 + 61*n2
                   - (77*n6 + 175*n3) */
    seq_t pos = 42 * n10 + 42 * n9 + 240 * n8 + 294 * n7 + 280 * n5 + 175 * n4 + 61 * n2;
    seq_t neg = 77 * n6 + 175 * n3;
    return (pos - neg) / 42;
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n) {
    if (n <= cache_size) return;

    /* Build initial 11 terms */
    if (cache == NULL) {
        cache_size = 11;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = compute_base(i);
    }

    /* Extend the cache up to n */
    if (n > cache_size) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = compute_base(i);
            } else {
                seq_t a1  = cache[i-1];
                seq_t a2  = cache[i-2];
                seq_t a3  = cache[i-3];
                seq_t a4  = cache[i-4];
                seq_t a5  = cache[i-5];
                seq_t a6  = cache[i-6];
                seq_t a7  = cache[i-7];
                seq_t a8  = cache[i-8];
                seq_t a9  = cache[i-9];
                seq_t a10 = cache[i-10];
                seq_t a11 = cache[i-11];

                seq_t pos = 11 * a1 + 165 * a3 + 462 * a5 + 330 * a7 + 55 * a9 + a11;
                seq_t neg = 55 * a2 + 330 * a4 + 462 * a6 + 165 * a8 + 11 * a10;
                cache[i] = pos - neg;
            }
        }
        cache_size = n;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for offset-less indices */
    ensure_cache(n);
    return cache[n];
}
