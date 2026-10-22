/*
 * A383952  Number of ways to tile a 1 X n strip using squares, red dominos,
 *          and blue dominos, where the number of red dominos must be twice
 *          the number of blues.
 * https://oeis.org/A383952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/6⌋} binomial(n-3k,3k) * binomial(3k,k)
 *            Computed via the D‑finite recurrence: (8n²-24n)a(n) =
 *            (28n²-108n+64)a(n-1) -(36n²-174n+180)a(n-2)
 *            +(20n²-120n+170)a(n-3) -(4n²-30n+54)a(n-4)
 *            +(54n²-324n+432)a(n-6) -(27n²-243n+540)a(n-7) (valid for n≥7).
 *            The initial values are a(0)=a(1)=a(2...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 24, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    const idx_t base = 7; /* we have 7 initial terms */

    /* return immediately for known small indices */
    if (n < base)
        return (const seq_t[7]){1,1,1,1,1,1,4}[n];

    /* initialise cache with the first 7 terms */
    if (cache_len < base) {
        cache_len = base;
        cache = (seq_t *)calloc((size_t)cache_len, sizeof(seq_t));
        /* initialise first 7 terms */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        cache[5] = 1;
        cache[6] = 4;
    }

    /* if requested term already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* extend the cache to hold up to index n */
    cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    cache_len = n + 1;

    /* compute missing terms using the recurrence */
    for (idx_t i = base; i <= n; ++i) {
        seq_t sn = (seq_t)i;                /* i as seq_t for coefficient calc */

        /* coefficients of the recurrence (in seq_t) */
        seq_t coef1 = (seq_t)28 * sn * sn - (seq_t)108 * sn + 64;
        seq_t coef2 = (seq_t)36 * sn * sn - (seq_t)174 * sn + 180;
        seq_t coef3 = (seq_t)20 * sn * sn - (seq_t)120 * sn + 170;
        seq_t coef4 = (seq_t) 4 * sn * sn - (seq_t) 30 * sn + 54;
        seq_t coef5 = (seq_t)54 * sn * sn - (seq_t)324 * sn + 432;
        seq_t coef6 = (seq_t)27 * sn * sn - (seq_t)243 * sn + 540;
        seq_t denom   = (seq_t)8 * sn * sn - (seq_t)24 * sn; /* >0 for i≥7 */

        /* positive and negative contributions separately to avoid
           underflow in the subtraction.  The recurrence guarantees
           that the positive sum is at least as large as the negative.    */
        seq_t pos = 0, neg = 0;
        pos += coef1 * cache[i - 1];
        pos += coef3 * cache[i - 3];
        pos += coef5 * cache[i - 6];

        neg += coef2 * cache[i - 2];
        neg += coef4 * cache[i - 4];
        neg += coef6 * cache[i - 7];

        cache[i] = (pos - neg) / denom;
    }

    return cache[n];
}
