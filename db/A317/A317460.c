/*
 * A317460  Number of nX3 0..1 arrays with every element unequal to 2, 3, 4,
 *          5, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence a(n) = 5*a(n-1) + 8*a(n-2) +
 *            2*a(n-3) - 48*a(n-4) - 62*a(n-5) + 35*a(n-6) + 62*a(n-7) +
 *            5*a(n-8) - 25*a(n-9) + 2*a(n-10) + 14*a(n-11) + 4*a(n-12)
 *            initial values: a(1)=0, a(2)=5, a(3)=16, a(4)=107, a(5)=636,
 *            a(6)=3893, a(7)=23769, a(8)=145280, a(9)=887980, a(10)=...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization table, index 0 unused */
static idx_t  cached_max = 0;        /* highest index currently stored */

/* base values for indices 1..14 */
static const seq_t base_vals[15] = {
    0,                      /* index 0, unused */
    0,                      /* a(1) */
    5,                      /* a(2) */
    16,                     /* a(3) */
    107,                    /* a(4) */
    636,                    /* a(5) */
    3893,                   /* a(6) */
    23769,                  /* a(7) */
    145280,                 /* a(8) */
    887980,                 /* a(9) */
    5428139,                /* a(10) */
    33181694,               /* a(11) */
    202838254,              /* a(12) */
    1239944574,             /* a(13) */
    7579749355              /* a(14) */
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined below offset, return 0 */

    /* Ensure the cache holds at least indices up to n */
    if (n > cached_max) {
        idx_t new_max = n;
        /* allocate/reallocate cache */
        if (cached_max == 0) {
            cache = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (new_max + 1) * sizeof(seq_t));
        }
        /* initialize new slots to 0 */
        for (idx_t i = cached_max + 1; i <= new_max; ++i)
            cache[i] = 0;
        cached_max = new_max;

        /* fill base values up to 14 if needed */
        if (cached_max >= 14) {
            for (idx_t i = 1; i <= 14; ++i)
                cache[i] = base_vals[i];
        } else if (cached_max >= 1) {
            for (idx_t i = 1; i <= cached_max; ++i)
                cache[i] = base_vals[i];
        }
    }

    if (n <= 14)
        return cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = 15; i <= n; ++i) {
        seq_t pos = 5 * cache[i - 1] + 8 * cache[i - 2] + 2 * cache[i - 3]
                    + 35 * cache[i - 6] + 62 * cache[i - 7] + 5 * cache[i - 8]
                    + 2 * cache[i - 10] + 14 * cache[i - 11] + 4 * cache[i - 12];
        seq_t neg = 48 * cache[i - 4] + 62 * cache[i - 5] + 25 * cache[i - 9];
        cache[i] = pos - neg;   /* subtraction is safe because pos >= neg */
    }
    return cache[n];
}
