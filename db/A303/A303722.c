/*
 * A303722  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2 or
 *          3 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 66, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) - 2*a(n-5) + 2*a(n-6)
   for n > 9, with initial values:
   a(1)=4, a(2)=21, a(3)=23, a(4)=46, a(5)=77, a(6)=151,
   a(7)=294, a(8)=549, a(9)=1051 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;  /* capacity in elements (0..cache_cap-1) */
    static idx_t  computed_n = 0;  /* highest index computed (0 if none) */

    /* Initialise cache on first call.  Allocate capacity for at least
       indices 0..9, then store the known initial values. */
    if (!cache) {
        /* Ensure we always have room for n=9 as a minimum */
        size_t init_cap = (size_t)(n > 9 ? n + 1 : 10);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* abort on allocation failure */
        cache_cap = init_cap;

        /* index 0 unused */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)23;
        cache[4] = (seq_t)46;
        cache[5] = (seq_t)77;
        cache[6] = (seq_t)151;
        cache[7] = (seq_t)294;
        cache[8] = (seq_t)549;
        cache[9] = (seq_t)1051;

        computed_n = 9;
    }

    /* Expand cache if the requested index exceeds current capacity */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= computed_n)
        return cache[n];

    /* Compute missing terms up to n (recurrence requires n>9, computed_n>=9) */
    for (idx_t i = computed_n + 1; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1];
        val += cache[i - 3];
        val -= 2 * cache[i - 4];
        val -= 2 * cache[i - 5];
        val += 2 * cache[i - 6];
        cache[i] = val;
    }

    computed_n = n;
    return cache[n];
}
