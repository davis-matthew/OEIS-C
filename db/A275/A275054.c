/*
 * A275054  G.f.: 3F2([1/9, 2/9, 8/9], [2/3,1], 729 x).
 * https://oeis.org/A275054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the recurrence * n^2*(3*n-1)*a(n) =
 *            3*(9*n-8)*(9*n-7)*(9*n-1)*a(n-1) * with a(0)=1 * (The recurrence
 *            uses only integer arithmetic; the division is exact.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* number of slots allocated */
    static idx_t last_filled = 0;         /* highest n for which cache is valid */

    /* Ensure capacity for indices 0..n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Base value: a(0) = 1 */
    if (cache_cap == 0) {
        cache[0] = (seq_t)1;
        cache_cap = 1;
        last_filled = 0;
    } else if (cache[0] == 0) {
        cache[0] = (seq_t)1;
    }

    /* Compute missing terms up to n */
    for (idx_t k = last_filled + 1; k <= n; ++k) {
        seq_t num = (seq_t)3 * (seq_t)(9 * k - 8);
        num = num * (seq_t)(9 * k - 7) * (seq_t)(9 * k - 1);
        seq_t den = (seq_t)k * (seq_t)k * (seq_t)(3 * k - 1);

        /* Cancel gcd with the previous value (a(k-1)) */
        seq_t g1 = gcd(cache[k - 1], den);
        seq_t prev_reduced = cache[k - 1] / g1;
        seq_t den1 = den / g1;

        /* Cancel remaining gcd between numerator and the remaining denominator */
        seq_t g2 = gcd(num, den1);
        seq_t num_reduced = num / g2;
        seq_t den2 = den1 / g2;  /* should be 1 after exact division */

        /* Compute a(k) */
        cache[k] = prev_reduced * num_reduced;  /* den2 == 1 */

        /* In the extremely unlikely event den2 != 1 we handle it */
        if (den2 != (seq_t)1) {
            cache[k] /= den2;
        }
    }

    last_filled = n;
    return cache[n];
}
