/*
 * A256639  a(n) is the base-(-10) representation of a(n-1) read in base 10,
 *          starting from -1.
 * https://oeis.org/A256639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(1) = -1; for n > 1, a(n) is obtained by converting a(n-1) to
 *            base -10, then interpreting that digit string as a decimal
 *            integer
 * Range:     i8 n <= 2, i16 n <= 13, i32 n <= 38, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t next_term(seq_t x)
{
    if (x == 0)
        return 0;
    seq_t res     = 0;
    seq_t factor  = 1;
    while (x != 0) {
        seq_t r = x % 10;
        if (r < 0)
            r += 10;
        res += r * factor;
        factor *= 10;
        seq_t tmp = x - r;
        x = tmp / -10;
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */

    if (n == 0)
        return 0;                  /* out of range, not used */

    if (n <= cache_len)
        return cache[n - 1];

    /* Allocate or grow cache */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * n);
        cache[0] = -1;             /* a(1) = -1 */
        cache_len = 1;
    } else if (n > cache_len) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i < n; ++i)
        cache[i] = next_term(cache[i - 1]);

    cache_len = n;
    return cache[n - 1];
}
