/*
 * A281750  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 395", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A281750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pk - a(n-1) + a(n-3) + a(n-4) + 256*a(n-8) + 256*a(n-9) -
 *            256*a(n-11) - 256*a(n-12) where pk =
 *            1,2,8,10,31,44,128,168,248,192,0,160,128,0,0,512,0,-512,-512,-512
 *            for n = 0..19 and pk = 0 for n > 19
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Coefficients of the numerator polynomial P(x) in the generating function */
static const int64_t p_coeff[20] = {
    1,         /* p0  */
    2,         /* p1  */
    8,         /* p2  */
    10,        /* p3  */
    31,        /* p4  */
    44,        /* p5  */
    128,       /* p6  */
    168,       /* p7  */
    248,       /* p8  */
    192,       /* p9  */
    0,         /* p10 */
    160,       /* p11 */
    128,       /* p12 */
    0,         /* p13 */
    0,         /* p14 */
    512,       /* p15 */
    0,         /* p16 */
   -512,       /* p17 */
   -512,       /* p18 */
   -512        /* p19 */
};

/* Memoisation cache */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (!cache || need > cache_cap) {
        size_t new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure – caller can handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        seq_t add = 0, sub = 0;
        int64_t pk = (k < 20) ? p_coeff[k] : 0;

        if (pk >= 0) add += (seq_t)pk;
        else          sub += (seq_t)(-pk);

        if (k >= 1)   sub += cache[k-1];
        if (k >= 3)   add += cache[k-3];
        if (k >= 4)   add += cache[k-4];
        if (k >= 8)   add += cache[k-8] * (seq_t)256;
        if (k >= 9)   add += cache[k-9] * (seq_t)256;
        if (k >= 11)  sub += cache[k-11] * (seq_t)256;
        if (k >= 12)  sub += cache[k-12] * (seq_t)256;

        cache[k] = add - sub;              /* guaranteed to be non‑negative */
    }
    cache_len = need;
    return cache[(size_t)n];
}
