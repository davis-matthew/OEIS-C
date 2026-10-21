/*
 * A269636  Number of length-n 0..4 arrays with no repeated value differing
 *          from the previous repeated value by other than plus two or minus
 *          1.
 * https://oeis.org/A269636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 13*a(n-1) - 51*a(n-2) + 15*a(n-3) + 240*a(n-4)
 *            - 96*a(n-5) - 509*a(n-6) - 268*a(n-7)
 * Initial values:
 *   a(1) = 5,  a(2) = 25,  a(3) = 120,  a(4) = 567,
 *   a(5) = 2637, a(6) = 12125, a(7) = 55225
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed = 0;

    static const int coeffs[7] = {13, -51, 15, 240, -96, -509, -268};
    static const seq_t init[7] = {
        (seq_t)5, (seq_t)25, (seq_t)120, (seq_t)567,
        (seq_t)2637, (seq_t)12125, (seq_t)55225
    };

    /* Expand cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    while (computed < n) {
        seq_t val;
        if (computed < 7) {
            val = init[computed];
        } else {
            val = 0;
            for (int k = 0; k < 7; ++k) {
                seq_t term = cache[computed - 1 - k];
                int c = coeffs[k];
                if (c >= 0)
                    val += term * (seq_t)c;
                else
                    val -= term * (seq_t)(-c);
            }
        }
        cache[computed] = val;
        ++computed;
    }

    return cache[(size_t)n - 1];
}
