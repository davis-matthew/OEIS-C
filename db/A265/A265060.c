/*
 * A265060  Coordination sequence for (2,4,5) tiling of hyperbolic plane.
 * https://oeis.org/A265060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) - a(n-3) - a(n-4) -
 *            a(n-5) + a(n-8) = 0 (n > 8) with initial values a(0)=1, a(1)=3,
 *            a(2)=5, a(3)=8, a(4)=12, a(5)=16, a(6)=21, a(7)=28, a(8)=36. The
 *            generating function is
 *            (x+1)^2*(x^2+1)*(x^4+x^3+x^2+x+1)/(x^8-x^5-x^4-x^3+1)
 * Range:     u8 n <= 15, u16 n <= 38, u32 n <= 83, u64 n <= 172,
 *            u128 n <= 352, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial values for n = 0..7 */
static const seq_t init_vals[8] = {1, 3, 5, 8, 12, 16, 21, 28};

/* coefficients of the numerator (0 <= n <= 8), zero for n>8 */
static const seq_t numer_coef[9] = {1, 3, 5, 7, 8, 7, 5, 3, 1};

static seq_t *cache = NULL;       /* dynamic cache of sequence values */
static idx_t cache_size = 0;      /* current size of the cache */

/* ensure that cache[n] is valid */
static void ensure_cache(idx_t n)
{
    if (cache != NULL && n < cache_size) return;

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);

    if (cache == NULL) {
        /* fill initial terms */
        for (idx_t i = 0; i < 8 && i < new_size; ++i)
            new_cache[i] = init_vals[i];

        /* compute remaining terms up to n */
        for (idx_t i = 8; i < new_size; ++i) {
            seq_t val = new_cache[i - 3] + new_cache[i - 4] + new_cache[i - 5]
                        - new_cache[i - 8];
            if (i <= 8) val += numer_coef[i];  /* numerator contribution only for n <= 8 */
            new_cache[i] = val;
        }
        cache = new_cache;
        cache_size = new_size;
    } else {
        /* extend existing cache */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 8) {
                new_cache[i] = init_vals[i];
            } else {
                seq_t val = new_cache[i - 3] + new_cache[i - 4] + new_cache[i - 5]
                            - new_cache[i - 8];
                if (i <= 8) val += numer_coef[i];
                new_cache[i] = val;
            }
        }
        cache = new_cache;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
