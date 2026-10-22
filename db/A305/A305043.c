/*
 * A305043  Number of nX4 0..1 arrays with every element unequal to 0, 2, 3, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 40, u64 n <= 76, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 2*a(n-2) + 4*a(n-3) - 12*a(n-4)
   + 6*a(n-5) + 2*a(n-6) + 3*a(n-7) - 3*a(n-9)   for n > 13
   Initial values:
   a(1)=1,a(2)=13,a(3)=9,a(4)=11,a(5)=20,a(6)=17,a(7)=25,a(8)=70,
   a(9)=67,a(10)=101,a(11)=292,a(12)=325,a(13)=485 */

#include <stdlib.h>

static const seq_t init_values[13] = {
    (seq_t)1, (seq_t)13, (seq_t)9,  (seq_t)11, (seq_t)20,
    (seq_t)17, (seq_t)25, (seq_t)70, (seq_t)67, (seq_t)101,
    (seq_t)292, (seq_t)325, (seq_t)485
};

static seq_t *cache   = NULL;  /* a(1) stored at cache[0] */
static size_t cache_cap   = 0;  /* allocated slots */
static size_t cache_calc  = 0;  /* highest n computed (1‑based) */

/* ensure the cache can hold at least 'needed' terms */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    cache_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    cache = tmp;
}

/* main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0u)        /* sequence offset 1: a(1) is first term */
        return (seq_t)0;

    size_t target = (size_t)n;

    /* Make sure enough space */
    ensure_cache(target);

    /* Fill any missing initial terms */
    size_t up_to = target < 13 ? target : 13;
    for (size_t i = cache_calc + 1; i <= up_to; ++i)
        cache[i - 1] = init_values[i - 1];
    if (up_to > cache_calc)
        cache_calc = up_to;

    /* Compute terms via recurrence */
    for (size_t i = cache_calc + 1; i <= target; ++i) {
        /* i >= 14 here */
        seq_t val =
            (seq_t)3 * cache[i - 2]        /* a(i-1) */
          - (seq_t)2 * cache[i - 3]        /* a(i-2) */
          + (seq_t)4 * cache[i - 4]        /* a(i-3) */
          - (seq_t)12 * cache[i - 5]       /* a(i-4) */
          + (seq_t)6 * cache[i - 6]        /* a(i-5) */
          + (seq_t)2 * cache[i - 7]        /* a(i-6) */
          + (seq_t)3 * cache[i - 8]        /* a(i-7) */
          - (seq_t)3 * cache[i - 10];      /* a(i-9) */
        cache[i - 1] = val;
    }
    cache_calc = target;

    return cache[target - 1];
}
