/*
 * A302737  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 4 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            12*a(n-1)+20*a(n-2)-18*a(n-3)-326*a(n-4)-375*a(n-5)+241*a(n-6)+1721*a(n-7)+1034*a(n-8)-798*a(n-9)-966*a(n-10)+86*a(n-11)+278*a(n-12)-242*a(n-13)-348*a(n-14)-112*a(n-15);
 *            Initial values: a(1)=8, a(2)=128, a(3)=1637, a(4)=21625,
 *            a(5)=286631, a(6)=3798398, a(7)=50347423, a(8)=667361051,
 *            a(9)=...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Do nothing for the unused zero index */
    if (n == 0) return 0;

    /* The recurrence has 15 terms; we keep a cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of elements stored in cache */

    /* Initial values up to n = 15 */
    static const seq_t init[15] = {
        8, 128, 1637, 21625, 286631,
        3798398, 50347423, 667361051,
        8845980434, 117255120887, 1554238778426,
        20601729761966, 273079838843598,
        3619725111303264, 47980143627858744
    };
    /* Coefficients of the recurrence */
    static const int coeffs[15] = {
        12, 20, -18, -326, -375, 241, 1721, 1034,
        -798, -966, 86, 278, -242, -348, -112
    };

    /* Ensure we have all initial values cached */
    if (cached < 15) {
        size_t need = 15;
        if (cache == NULL) {
            cache = (seq_t*)malloc(need * sizeof(seq_t));
            if (!cache) return 0;     /* allocation failed */
        } else if (cached < need) {
            seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
            if (!tmp) return 0;      /* allocation failed */
            cache = tmp;
        }
        for (idx_t i = cached; i < 15; ++i) cache[i] = init[i];
        cached = 15;
    }

    /* Return immediately if we already have the value */
    if (n <= cached) return cache[n-1];

    /* Grow cache to the requested size */
    if (cache == NULL) {
        /* Should not happen because we already allocated above */
        size_t need = n;
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!cache) return 0;
        for (idx_t i = 0; i < 15; ++i) cache[i] = init[i];
        cached = 15;
    } else if (cached < n) {
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        /* missing entries beyond previous cached size are uninitialized; they will be filled below */
    }

    /* Compute successive terms using the recurrence */
    for (idx_t i = cached; i < n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 15; ++k) {
            seq_t v = cache[i-1-k];
            int c = coeffs[k];
            if (c >= 0)
                pos += v * (seq_t)c;
            else
                neg += v * (seq_t)(-c);
        }
        cache[i] = pos - neg;
    }
    cached = n;
    return cache[n-1];
}
