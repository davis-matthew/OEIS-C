/*
 * A301420  Sums of positive coefficients in generalized Chebyshev polynomials
 *          of the first kind, for a family of 5 data.
 * https://oeis.org/A301420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = linear recurrence a(n) = 7*a(n-1) - a(n-2) - 6*a(n-3) +
 *            2*a(n-4) - a(n-5) + 4*a(n-6) + a(n-7) + 4*a(n-9) + 2*a(n-10) -
 *            a(n-11) - 5*a(n-12) - 4*a(n-13) - a(n-14) for n ≥ 15 Initial
 *            values (a(1) … a(14)): 1, 5, 31, 205, 1376, 9251, 62210, 418361,
 *            2813485, 18920751, 127242501, 855708865, 5754...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initial 14 terms, index 1..14 */
    static const seq_t init[15] = {0,
        1ull,
        5ull,
        31ull,
        205ull,
        1376ull,
        9251ull,
        62210ull,
        418361ull,
        2813485ull,
        18920751ull,
        127242501ull,
        855708865ull,
        5754662616ull,
        38700243965ull
    };

    /* first time: allocate space for indices 0..14 */
    if (cache_len == 0) {
        cache_len = 15;               /* indices 0 and 1..14 used */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();          /* memory failure – fatal */
        for (idx_t i = 0; i < cache_len; ++i) cache[i] = 0;
        for (idx_t i = 1; i <= 14; ++i) cache[i] = init[i];
    }

    /* if we already have the value, just return it */
    if (n < cache_len) return cache[n];

    /* extend cache up to needed index */
    idx_t new_len = n + 1;
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();           /* memory failure – fatal */
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        cache_len = new_len;
    }

    /* compute new values from old_len to n */
    for (idx_t i = 15; i <= n; ++i) {
        seq_t sum = 0, sub = 0;
        /* sum  = 7*a[i-1] + 2*a[i-4] + 4*a[i-6] + a[i-7] + 4*a[i-9] + 2*a[i-10] */
        sum += 7 * cache[i - 1];
        sum += 2 * cache[i - 4];
        sum += 4 * cache[i - 6];
        sum +=       cache[i - 7];
        sum += 4 * cache[i - 9];
        sum += 2 * cache[i -10];
        /* sub  = a[i-2] + 6*a[i-3] + a[i-5] + a[i-11] + 5*a[i-12] + 4*a[i-13] + a[i-14] */
        sub +=       cache[i - 2];
        sub += 6 * cache[i - 3];
        sub +=       cache[i - 5];
        sub +=       cache[i -11];
        sub += 5 * cache[i -12];
        sub += 4 * cache[i -13];
        sub +=       cache[i -14];
        cache[i] = sum - sub;
    }

    return cache[n];
}
