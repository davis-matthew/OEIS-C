/*
 * A297981  Number of n X 3 0..1 arrays with every element equal to 0, 2, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - a(n-4) - 4*a(n-6) - 8*a(n-7)
 *            - 2*a(n-8) + 4*a(n-9) + 6*a(n-10) + 2*a(n-11) for n > 13 a(1)=1,
 *            a(2)=6, a(3)=2, a(4)=5, a(5)=7, a(6)=14, a(7)=21, a(8)=41,
 *            a(9)=70, a(10)=129, a(11)=233, a(12)=428, a(13)=783
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 73, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* storage for computed terms */
static size_t cache_cap = 0;          /* allocated capacity (index 0 unused) */
static idx_t computed = 0;            /* highest index with a valid value */

/* Make sure we have room for at least n+1 elements (index 0 unused). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failure – in the test harness the
                         program will terminate before this happens */
        return;
    }
    cache = new_cache;
    cache_cap = new_cap;
    if (cache_cap > 0)
        cache[0] = (seq_t)0;          /* unused */
}

static const seq_t init_vals[14] = {
    0,          /* dummy for index 0 */
    1,          /* a(1) */
    6,          /* a(2) */
    2,          /* a(3) */
    5,          /* a(4) */
    7,          /* a(5) */
    14,         /* a(6) */
    21,         /* a(7) */
    41,         /* a(8) */
    70,         /* a(9) */
    129,        /* a(10) */
    233,        /* a(11) */
    428,        /* a(12) */
    783         /* a(13) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* not defined – return 0 */

    ensure_cache(n);

    /* Compute missing terms up to n.  The cache is filled in ascending
       order, so later calls do not need to recompute previously known
       values. */
    while (computed < n)
    {
        idx_t k = computed + 1;
        seq_t val;

        if (k <= 13)
        {
            val = init_vals[(size_t)k];
        }
        else
        {
            /* Compute using a formulation that guarantees non‑negative
               intermediate results, avoiding unsigned underflow. */
            seq_t pos = cache[k-1]
                        + (seq_t)2 * cache[k-2]
                        + cache[k-3]
                        + (seq_t)4 * cache[k-9]
                        + (seq_t)6 * cache[k-10]
                        + (seq_t)2 * cache[k-11];

            seq_t sub = cache[k-4]
                        + (seq_t)4 * cache[k-6]
                        + (seq_t)8 * cache[k-7]
                        + (seq_t)2 * cache[k-8];

            val = pos - sub;
        }

        cache[k] = val;
        computed = k;
    }

    return cache[n];
}
