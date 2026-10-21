/*
 * A276126  a(0) = a(1) = a(2) = a(3) = a(4) = 1; for n>4, a(n) = (
 *          a(n-1)^2+a(n-2)^2+a(n-3)^2+a(n-4)^2+a(n-1)*a(n-2)*a(n-3)*a(n-4) )
 *          / a(n-5).
 * https://oeis.org/A276126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^2 + a(n-2)^2 + a(n-3)^2 + a(n-4)^2 +
 *            a(n-1)*a(n-2)*a(n-3)*a(n-4)) / a(n-5) Initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cache[0 .. cache_max] */
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t cache_max = 0;           /* highest index computed so far */
static int cache_initialized = 0;     /* flag */

static void
ensure_cache_capacity(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    size_t need = cache_cap ? cache_cap : 16ULL;
    while (need <= (size_t)n)
        need <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (new_cache == NULL) {
        abort();
    }
    cache = new_cache;
    cache_cap = need;
}

seq_t a(idx_t n)
{
    if (!cache_initialized) {
        ensure_cache_capacity(4);
        cache[0] = cache[1] = cache[2] = cache[3] = cache[4] = (seq_t)1;
        cache_max = 4;
        cache_initialized = 1;
    }

    if (n <= cache_max)
        return cache[n];

    ensure_cache_capacity(n);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];

        seq_t num = a1 * a1 + a2 * a2 + a3 * a3 + a4 * a4
                    + a1 * a2 * a3 * a4;

        cache[i] = num / a5;   /* exact division by definition */
    }

    cache_max = n;
    return cache[n];
}
