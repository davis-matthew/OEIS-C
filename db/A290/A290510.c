/*
 * A290510  Number of irredundant sets in the n-antiprism graph.
 * https://oeis.org/A290510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)+5*a(n-3)-6*a(n-5)-10*a(n-6)
 *            +6*a(n-8)+10*a(n-9)-a(n-10)-2*a(n-11)-5*a(n-12)+a(n-15) for
 *            n>15. Initial values: a(1)=1, a(2)=5, a(3)=22, a(4)=37, a(5)=76,
 *            a(6)=194, a(7)=491, a(8)=1125, a(9)=2731, a(10)=6640,
 *            a(11)=15962, a(12)=38386, a(13)=92639, a(14)=223403,
 *            a(15)=538102
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[16] = {
    /* 0 unused                                        */
    0ULL, /* dummy for index 0 */
    1ULL, 5ULL, 22ULL, 37ULL, 76ULL,
    194ULL, 491ULL, 1125ULL, 2731ULL, 6640ULL,
    15962ULL, 38386ULL, 92639ULL, 223403ULL, 538102ULL
};

static seq_t *cache = NULL;   /* dynamic cache of computed terms */
static idx_t cached_size = 0; /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) is not defined in the OEIS sequence; return 0 for safety */
        return (seq_t)0;
    }

    /* Expand cache if necessary */
    if (n > cached_size) {
        /* compute up to index n */
        idx_t i;
        idx_t old_size = cached_size;
        idx_t new_size = n; /* we need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed: return 0 (caller must handle error) */
            return (seq_t)0;
        }
        cache = tmp;

        /* initialize new entries */
        for (i = old_size + 1; i <= n; ++i) {
            if (i <= 15) {
                /* use the precomputed initial values */
                cache[i] = init_values[i];
            } else {
                /* recurrence for n > 15 */
                cache[i] =  cache[i-1] + 2*cache[i-2] + 5*cache[i-3]
                          - 6*cache[i-5] - 10*cache[i-6]
                          + 6*cache[i-8] + 10*cache[i-9]
                          - cache[i-10] - 2*cache[i-11] - 5*cache[i-12]
                          + cache[i-15];
            }
        }

        cached_size = n;
    }

    return cache[n];
}
