/*
 * A321614  Number of nonequivalent ways to place 2n nonattacking kings on a 4
 *          X 2n chessboard under all symmetry operations of the rectangle.
 * https://oeis.org/A321614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 12*a(n-1) - 54*a(n-2) + 98*a(n-3) + 17*a(n-4) -
 *            346*a(n-5) + 505*a(n-6) - 210*a(n-7) - 120*a(n-8) + 126*a(n-9) -
 *            27*a(n-10) a(0)=1, a(1)=4, a(2)=23, a(3)=106, a(4)=473,
 *            a(5)=1939, a(6)=7618, a(7)=28703, a(8)=105112, a(9)=375597
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t init_vals[10] =
    {1, 4, 23, 106, 473, 1939, 7618, 28703, 105112, 375597};

static seq_t *cache = NULL;      /* memoisation array */
static size_t cache_size = 0;    /* allocated size */
static idx_t last_computed = 0;  /* highest index stored in cache */

/* Initialise the cache with the first 10 terms. */
static void init_cache(void) {
    cache_size = 10;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
    for (size_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
    last_computed = 9;
}

/* Grow the cache to at least 'limit + 1' elements. */
static void ensure_capacity(idx_t limit) {
    if (!cache) init_cache();                /* first allocation */
    if ((size_t)(limit + 1) <= cache_size) return;
    size_t new_size = cache_size ? cache_size : 1;
    while (new_size <= (size_t)(limit + 1)) new_size <<= 1;
    seq_t *p = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!p) exit(1);
    cache = p;
    cache_size = new_size;
}

/* Compute terms up to 'limit' using the recurrence. */
static void compute_up_to(idx_t limit) {
    if (!cache) init_cache();
    if (limit <= last_computed) return;
    for (idx_t i = last_computed + 1; i <= limit; ++i) {
        /* Use only addition and subtraction with unsigned types
           to avoid intermediate underflow. */
        seq_t pos = (seq_t)12 * cache[i-1]
                  + (seq_t)98 * cache[i-3]
                  + (seq_t)17 * cache[i-4]
                  + (seq_t)505 * cache[i-6]
                  + (seq_t)126 * cache[i-9];
        seq_t neg = (seq_t)54 * cache[i-2]
                  + (seq_t)346 * cache[i-5]
                  + (seq_t)210 * cache[i-7]
                  + (seq_t)120 * cache[i-8]
                  + (seq_t)27 * cache[i-10];
        cache[i] = pos - neg;
    }
    last_computed = limit;
}

seq_t a(idx_t n) {
    if (n < 10) return init_vals[(size_t)n];
    ensure_capacity(n);
    compute_up_to(n);
    return cache[n];
}
