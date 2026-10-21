/*
 * A299368  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 4,
 *          5 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 2*a(n-2) −19*a(n-3) − a(n-4) −2*a(n-5) +
 *            4*a(n-6) * −2*a(n-7) −3*a(n-8) −5*a(n-9) + 2*a(n-10) for n>11 *
 *            Initial values: a(1)=1, a(2)=18, a(3)=130, a(4)=876, a(5)=6025,
 *            * a(6)=41528, a(7)=285931, a(8)=1968966, a(9)=13558267, *
 *            a(10)=93362733, a(11)=642898196
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

/* Initial terms, index 0 unused */
static const seq_t init[12] = {
    0ULL,          /* dummy 0 index */
    1ULL,          /* a(1) */
    18ULL,         /* a(2) */
    130ULL,        /* a(3) */
    876ULL,        /* a(4) */
    6025ULL,       /* a(5) */
    41528ULL,      /* a(6) */
    285931ULL,     /* a(7) */
    1968966ULL,    /* a(8) */
    13558267ULL,   /* a(9) */
    93362733ULL,   /* a(10) */
    642898196ULL   /* a(11) */
};

/* Cache for computed values */
static seq_t *cache   = NULL;   /* cache[i] holds a(i) for 1 <= i <= cache_size */
static idx_t  cache_size = 0;   /* largest index currently cached */

/* --------------------------------------------------------------- */
/* Compute a(n) for any n >= 1, supporting arbitrary call order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* below offset */

    /* Initialise or grow cache if needed */
    if (cache == NULL || cache_size < n) {
        idx_t new_size = (n > 11) ? n : 11;     /* ensure at least first 11 terms */
        seq_t *new_cache = alloc_seq_array((size_t)new_size + 1); /* +1 for index 0 */
        if (new_cache == NULL)
            abort();

        /* Copy previous cached values */
        if (cache) {
            for (idx_t i = 1; i <= cache_size; ++i)
                new_cache[i] = cache[i];
            free_seq(cache);
        }

        /* Initialise first 11 terms */
        for (idx_t i = 1; i <= 11; ++i)
            new_cache[i] = init[i];

        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute terms beyond the initial 11 using the recurrence */
    if (n > 11) {
        for (idx_t i = 12; i <= n; ++i) {
            seq_t pos = cache[i-1] * (seq_t)7
                       + cache[i-2] * (seq_t)2
                       + cache[i-6] * (seq_t)4
                       + cache[i-10] * (seq_t)2;

            seq_t neg = cache[i-3] * (seq_t)19
                       + cache[i-4]
                       + cache[i-5] * (seq_t)2
                       + cache[i-7] * (seq_t)2
                       + cache[i-8] * (seq_t)3
                       + cache[i-9] * (seq_t)5;

            cache[i] = pos - neg;
        }
    }

    return cache[n];
}
