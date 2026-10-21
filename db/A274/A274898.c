/*
 * A274898  Number of 6Xn 0..2 arrays with no element equal to any value at
 *          offset (0,-1) (-1,-1) or (-2,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) -77*a(n-2) +286*a(n-3) -736*a(n-4) +1351*a(n-5)
 *            -1787*a(n-6) +1716*a(n-7) -1217*a(n-8) +652*a(n-9) -265*a(n-10)
 *            +78*a(n-11) -14*a(n-12) +a(n-13), for n > 14 with initial
 *            values: a(1)=24, a(2)=50, a(3)=192, a(4)=753, a(5)=3017,
 *            a(6)=12217, a(7)=49697, a(8)=202749, a(9)=828828, a(1...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[15] = {0,
    24, 50, 192, 753, 3017, 12217, 49697,
    202749, 828828, 3391310, 13878192,
    56789705, 232384474, 950977585};

static seq_t *cache = NULL;
static idx_t cache_len = 0;     /* largest index computed */
static size_t cache_cap = 0;    /* allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, return 0 */

    /* allocate or extend the cache if necessary */
    if (cache == NULL) {
        cache_cap = (n >= 14 ? n : 14) + 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        for (idx_t i = 1; i <= 14 && i <= n; ++i) cache[i] = init[i];
        cache_len = (n >= 14 ? 14 : n);
    }
    else if (cache_len < n) {
        /* ensure sufficient capacity */
        if (cache_cap < (size_t)n + 1) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)n + 1) new_cap = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 14) {
                cache[i] = init[i];
            } else {
                seq_t pos = cache[i-1] * 13;
                pos += cache[i-3] * 286;
                pos += cache[i-5] * 1351;
                pos += cache[i-7] * 1716;
                pos += cache[i-9] * 652;
                pos += cache[i-11] * 78;
                pos += cache[i-13];

                seq_t neg = cache[i-2] * 77;
                neg += cache[i-4] * 736;
                neg += cache[i-6] * 1787;
                neg += cache[i-8] * 1217;
                neg += cache[i-10] * 265;
                neg += cache[i-12] * 14;

                cache[i] = pos - neg;   /* pos >= neg for all n */
            }
        }
        cache_len = n;
    }

    return cache[n];
}
