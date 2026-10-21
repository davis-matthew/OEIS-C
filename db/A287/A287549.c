/*
 * A287549  Total number of unordered factorizations of all positive integers
 *          <= n into distinct factors greater than 1.
 * https://oeis.org/A287549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   count_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A287549
 *  a(n) = sum_{m = 1}^{n} g(m)
 *  where g(1) = 1 and for m > 1,
 *       g(m) = #{ unordered factorizations of m into
 *                  distinct integers > 1 }.
 *  This is equivalent to:
 *      g(m) = 1 + sum_{d | m , 2 <= d < m} g(m/d),
 *          with the constraint that the chosen divisor d
 *          is strictly greater than any previously chosen factor.
 */

#include <stdlib.h>
#include "../utils/count_factors.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1, no defined value below */

    static seq_t *sum_cache = NULL;
    static size_t cache_cap  = 0; /* largest n for which cache is valid */

    if (n > cache_cap) {
        /* grow cache to include entries up to n */
        size_t new_cap = (size_t)n + 1;           /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) return 0;        /* allocation failure */
        sum_cache = tmp;
        if (cache_cap == 0) {
            /* base case for cumulative sum */
            sum_cache[0] = 0;
        }
        for (size_t i = cache_cap + 1; i <= (size_t)n; ++i) {
            seq_t g_val;
            if (i == 1) {
                g_val = 1;                     /* f(1) = 1 */
            } else {
                g_val = count_factors((idx_t)i, 2);
            }
            sum_cache[i] = sum_cache[i - 1] + g_val;
        }
        cache_cap = (size_t)n;
    }

    return sum_cache[(size_t)n];
}
