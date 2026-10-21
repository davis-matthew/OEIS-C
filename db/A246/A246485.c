/*
 * A246485  Number of length 6+3 0..n arrays with no pair in any consecutive
 *          four terms totalling exactly n.
 * https://oeis.org/A246485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +4*a(n-2) -20*a(n-3) +56*a(n-5) -28*a(n-6)
 *            -84*a(n-7) +70*a(n-8) +70*a(n-9) -84*a(n-10) -28*a(n-11)
 *            +56*a(n-12) -20*a(n-14) +4*a(n-15) +3*a(n-16) -a(n-17) Initial
 *            values: a(1)=2, a(2)=52, a(3)=3132, a(4)=31104, a(5)=347934,
 *            a(6)=1893780, a(7)=9714968, a(8)=35251360, a(9)=119069850...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 139, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* 1‑based cache; cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of allocated slots (>= current max n+1) */

/* Allocate and fill the first 17 values (including a dummy 0 index). */
static void init_cache(void)
{
    cache = (seq_t *)malloc(18 * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)52;
    cache[3] = (seq_t)3132;
    cache[4] = (seq_t)31104;
    cache[5] = (seq_t)347934;
    cache[6] = (seq_t)1893780;
    cache[7] = (seq_t)9714968;
    cache[8] = (seq_t)35251360;
    cache[9] = (seq_t)119069850;
    cache[10] = (seq_t)331355220;
    cache[11] = (seq_t)869135892;
    cache[12] = (seq_t)2019732192;
    cache[13] = (seq_t)4480392182;
    cache[14] = (seq_t)9156814324;
    cache[15] = (seq_t)18041010864;
    cache[16] = (seq_t)33514806720;
    cache[17] = (seq_t)60460359858;

    cache_len = 18;   /* indices 0 … 17 allocated */
}

/* Ensure the cache is large enough. Existing data are unchanged. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_len) return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset 1 – undefined for n<1 */

    if (!cache) init_cache();             /* create the cache on first call */

    if ((size_t)n < cache_len)            /* already computed */
        return cache[n];

    /* compute values up to n */
    size_t old_len = cache_len;
    ensure_cache((size_t)n + 1);          /* allocate slots 0 … n */

    for (size_t i = old_len; i <= (size_t)n; ++i) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        /* positive contributions */
        pos += (seq_t)3   * cache[i-1];
        pos += (seq_t)4   * cache[i-2];
        pos += (seq_t)56  * cache[i-5];
        pos += (seq_t)70  * cache[i-8];
        pos += (seq_t)70  * cache[i-9];
        pos += (seq_t)56  * cache[i-12];
        pos += (seq_t)4   * cache[i-15];
        pos += (seq_t)3   * cache[i-16];

        /* negative contributions */
        neg += (seq_t)20  * cache[i-3];
        neg += (seq_t)28  * cache[i-6];
        neg += (seq_t)84  * cache[i-7];
        neg += (seq_t)84  * cache[i-10];
        neg += (seq_t)28  * cache[i-11];
        neg += (seq_t)20  * cache[i-14];
        neg +=          cache[i-17];

        cache[i] = pos - neg;
    }
    return cache[n];
}
