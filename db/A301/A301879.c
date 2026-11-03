/*
 * A301879  Number of nX3 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 2*a(n-3), a(1)=3, a(2)=10, a(3)=29
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static idx_t cached = 0;
    static size_t cap = 0;

    if (n <= cached)
        return cache[(size_t)(n - 1)];

    if (cache == NULL || cap < (size_t)n) {
        cap = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, cap);
        cache = tmp;
    }

    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i == 1)
            cache[0] = (seq_t)3;
        else if (i == 2)
            cache[1] = (seq_t)10;
        else if (i == 3)
            cache[2] = (seq_t)29;
        else {
            seq_t a1 = cache[(size_t)(i - 2)]; /* a(i-1) */
            seq_t a2 = cache[(size_t)(i - 3)]; /* a(i-2) */
            seq_t a3 = cache[(size_t)(i - 4)]; /* a(i-3) */
            cache[(size_t)(i - 1)] = a1 + ((seq_t)6 * a2) + ((seq_t)2 * a3);
        }
    }

    cached = n;
    return cache[(size_t)(n - 1)];
}
