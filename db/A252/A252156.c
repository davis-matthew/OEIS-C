/*
 * A252156  Number of (n+2)X(7+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not equal to 0 1 3 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum equal to 0 1 3 6 or 7.
 * https://oeis.org/A252156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = -a(n-1) + 2*a(n-2) +
 *            14*a(n-3) + 14*a(n-4) - 28*a(n-5) - 56*a(n-6) - 56*a(n-7) +
 *            112*a(n-8) + 64*a(n-9) + 64*a(n-10) - 128*a(n-11) for n > 16
 *            Initial values (offset 1): a(1)=13146, a(2)=16786, a(3)=35924,
 *            a(4)=58792, a(5)=114243, a(6)=223442, a(7)=44666...
 * Range:     u8 none, u16 n <= 4, u32 n <= 20, u64 n <= 52, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache[0] unused, cache[1] = a(1), ... */
static size_t cache_cap = 0;          /* allocated capacity (size) */
static idx_t  cache_max = 0;          /* largest index for which a(i) is stored */

static void ensure_cache(idx_t need)
{
    if (cache_cap >= (size_t)need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < (size_t)need)
        new_cap <<= 1;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;
}

/* Initialize base values 1 … 16 */
static void init_cache_if_needed(idx_t n)
{
    if (cache_max != 0)
        return;   /* already initialized */

    /* Ensure we have capacity for at least 17 entries (0..16) */
    ensure_cache(17);

    /* base values */
    cache[0] = 0; /* unused */
    cache[1] = (seq_t)13146;   cache[2] = (seq_t)16786;
    cache[3] = (seq_t)35924;   cache[4] = (seq_t)58792;
    cache[5] = (seq_t)114243;  cache[6] = (seq_t)223442;
    cache[7] = (seq_t)446661;  cache[8] = (seq_t)897932;
    cache[9] = (seq_t)1788360; cache[10] = (seq_t)3551084;
    cache[11] = (seq_t)7203948;cache[12] = (seq_t)14236172;
    cache[13] = (seq_t)28512252;cache[14] = (seq_t)57373724;
    cache[15] = (seq_t)114332028;cache[16] = (seq_t)227108060;

    cache_max = 16;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* offset 1 */

    /* Ensure cache is ready with base values */
    init_cache_if_needed(n);
    ensure_cache(n);

    /* Compute missing values */
    while (cache_max < n) {
        idx_t i = cache_max + 1;
        seq_t val = (seq_t)0;

        /* Apply recurrence */
        val += - cache[i-1];
        val +=  2 * cache[i-2];
        val += 14 * cache[i-3];
        val += 14 * cache[i-4];
        val += -28 * cache[i-5];
        val += -56 * cache[i-6];
        val += -56 * cache[i-7];
        val += 112 * cache[i-8];
        val +=  64 * cache[i-9];
        val +=  64 * cache[i-10];
        val += -128 * cache[i-11];

        cache[i] = val;
        cache_max = i;
    }
    return cache[n];
}
