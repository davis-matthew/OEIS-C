/*
 * A253150  Number of perfect matchings in the P_5 X C_{2n} graph.
 * https://oeis.org/A253150
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 192*a(n-2) + 703*a(n-3) - 1320*a(n-4) +
 *            1320*a(n-5) - 703*a(n-6) + 192*a(n-7) - 24*a(n-8) + a(n-9)
 *            Initial values: a(2)=450, a(3)=4480, a(4)=51842, a(5)=631750,
 *            a(6)=7840800, a(7)=97964230, a(8)=1227006722, a(9)=15382568320,
 *            a(10)=192913661250 The function is memoised and works...
 * Range:     u8 none, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const idx_t MAX_N = 10000;   /* practical limit for this implementation */

/* static memoisation storage */
static seq_t *cache = NULL;
static idx_t  cache_computed = 1;   /* largest index known (initially 1) */

/* initialise the cache the first time a() is called */
static void cache_init(void)
{
    if (cache != NULL)
        return;                       /* already initialised */

    /* allocate memory for indices 0 .. MAX_N inclusive */
#ifdef __cplusplus
    cache = new seq_t[MAX_N + 1];
    /* zero‑initialise all positions (the default constructor is then
       invoked for class types, and integral types are set to 0) */
    for (idx_t i = 0; i <= MAX_N; ++i)
        cache[i] = (seq_t)0;
#else
    cache = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
#endif
    /* set the known initial terms */
    if (cache != NULL) {
        cache[2] = (seq_t)450;
        cache[3] = (seq_t)4480;
        cache[4] = (seq_t)51842;
        cache[5] = (seq_t)631750;
        cache[6] = (seq_t)7840800;
        cache[7] = (seq_t)97964230;
        cache[8] = (seq_t)1227006722;
        cache[9] = (seq_t)15382568320;
        cache[10] = (seq_t)192913661250;
        cache_computed = 10;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* undefined for indices before the offset */

    if (cache == NULL)
        cache_init();

    if (n > MAX_N)
        return 0;   /* out of range for this implementation */

    /* compute any missing terms up to n using the recurrence */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        if (i <= 10)
            continue;   /* values already initialised */
        cache[i] = cache[i-1]*(seq_t)24
                 - cache[i-2]*(seq_t)192
                 + cache[i-3]*(seq_t)703
                 - cache[i-4]*(seq_t)1320
                 + cache[i-5]*(seq_t)1320
                 - cache[i-6]*(seq_t)703
                 + cache[i-7]*(seq_t)192
                 - cache[i-8]*(seq_t)24
                 + cache[i-9];
    }
    cache_computed = n;
    return cache[n];
}
