/*
 * A294781  Growth of the Lamplighter group: number of elements in the
 *          Lamplighter group Z wr Z of length up to n with respect to the
 *          standard generating set {a,t}.
 * https://oeis.org/A294781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - 4*a(n-4) + 4*a(n-5) +
 *            6*a(n-6) + 4*a(n-7) + a(n-8) (n >= 8) Initial values: a(0)=1,
 *            a(1)=5, a(2)=17, a(3)=53, a(4)=153, a(5)=421, a(6)=1125,
 *            a(7)=2937
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[n] holds a(n)          */
static idx_t  cache_size = 0;        /* number of elements already computed */
static size_t cache_cap   = 0;        /* allocated capacity */

static void ensure_capacity(idx_t n)
{
    if ((idx_t)(cache_cap) > n) return;          /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < (size_t)(n + 1)) new_cap = n + 1;

    seq_t *tmp = (cache ? (seq_t *)realloc(cache, new_cap * sizeof(seq_t))
                        : (seq_t *)malloc(new_cap * sizeof(seq_t)));
    if (!tmp) abort();                           /* out of memory */

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* lazy computation with caching */
    if (cache_size <= n) {
        ensure_capacity(n);
        static const seq_t base[8] = {
            1, 5, 17, 53, 153, 421, 1125, 2937
        };

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 8) {
                cache[i] = base[i];
            } else {
                /* recurrence: */
                seq_t res =   4 * cache[i-1]
                         - 2 * cache[i-2]
                         - 4 * cache[i-3]
                         - 4 * cache[i-4]
                         + 4 * cache[i-5]
                         + 6 * cache[i-6]
                         + 4 * cache[i-7]
                         +     cache[i-8];
                cache[i] = res;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
