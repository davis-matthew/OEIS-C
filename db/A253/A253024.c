/*
 * A253024  Number of (n+2)X(7+2) 0..3 arrays with every consecutive three
 *          elements in every row and column not having exactly two distinct
 *          values, and in every diagonal and antidiagonal having exactly two
 *          distinct values, and new values 0 upwards introduced in row major
 *          order.
 * https://oeis.org/A253024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3) - a(n-6) - a(n-9) - a(n-12) - a(n-15) for n >
 *            20, with initial values a(1)=189, a(2)=243, a(3)=302, a(4)=369,
 *            a(5)=554, a(6)=720, a(7)=920, a(8)=1312, a(9)=1616, a(10)=1976,
 *            a(11)=2784, a(12)=3472, a(13)=4272, a(14)=6096, a(15)=7664,
 *            a(16)=9464, a(17)=13504, a(18)=16952, a(19)=209...
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 65, u64 n <= 149, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static const seq_t init_values[20] = {
    189, 243, 302, 369, 554, 720, 920, 1312, 1616, 1976,
    2784, 3472, 4272, 6096, 7664, 9464, 13504, 16952, 20904, 29792
};

static seq_t *cache = NULL;          /* array of computed a(n) values */
static size_t cache_cap = 0;         /* storage capacity in elements */
static idx_t cache_size = 0;         /* number of values actually computed */

/* Ensure that the cache has capacity for at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();  /* out of memory */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1.  Sequence values are memoized. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 by convention */

    /* Indexing in cache is 0‑based: a(1) is stored at cache[0] */
    if (n <= cache_size)
        return cache[n - 1];

    ensure_cache_cap((size_t)n);

    /* Build missing entries */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 20) {
            val = init_values[i - 1];
        } else {
            /* a(i) = 3*a(i-3) - a(i-6) - a(i-9) - a(i-12) - a(i-15) */
            seq_t t1 = cache[i - 4];   /* a(i-3) */
            seq_t t2 = cache[i - 7];   /* a(i-6) */
            seq_t t3 = cache[i -10];   /* a(i-9) */
            seq_t t4 = cache[i -13];   /* a(i-12) */
            seq_t t5 = cache[i -16];   /* a(i-15) */
            val = 3 * t1 - t2 - t3 - t4 - t5;
        }
        cache[i - 1] = val;
    }
    cache_size = n;
    return cache[n - 1];
}
