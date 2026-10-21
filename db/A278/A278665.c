/*
 * A278665  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 62", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-4) - 10*a(n-5) + b_n where b_n is the
 *            coefficient of x^n in the numerator of the generating function:
 *            b_0 = 1, b_1 = 1, b_2 = -99, b_3 = -99, b_4 = -100, b_5 = 9900,
 *            b_6 = 10000, b_7 = -1000000, b_8 = 0, b_9 = 0, b_10 = 1000000,
 *            and b_n = 0 for n > 10
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void grow_cache(size_t needed)
{
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap <= needed) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
}

static void init_cache(void)
{
    if (cache_len > 0) return;          /* already initialized */
    grow_cache(11);                     /* enough for indices 0..10 */
    cache[0] = 1;
    cache[1] = 11;
    cache[2] = 11;
    cache[3] = 11;
    cache[4] = 11;
    cache_len = 5;                       /* indices 0..4 known */
}

static const int bn[11] = {
    1,          /* b_0  */
    1,          /* b_1  */
   -99,         /* b_2  */
   -99,         /* b_3  */
  -100,         /* b_4  */
   9900,        /* b_5  */
  10000,        /* b_6  */
 -1000000,     /* b_7  */
   0,          /* b_8  */
   0,          /* b_9  */
 1000000       /* b_10 */
};

seq_t a(idx_t n)
{
    init_cache();

    if (n < 5)           /* initial segment */
        return cache[n];

    if ( (idx_t)cache_len > n )
        return cache[n];

    /* compute sequentially from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] * 10;      /* 10 * a(i-1) */
        val += cache[i-4];                /* + a(i-4)   */
        val -= cache[i-5] * 10;           /* - 10*a(i-5)*/
        if (i <= 10) {
            int bi = bn[i];
            if (bi > 0)
                val += (seq_t)bi;
            else if (bi < 0)
                val -= (seq_t)(-bi);
        }
        if (i >= cache_cap) grow_cache(i);
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
