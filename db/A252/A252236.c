/*
 * A252236  Number of (n+2) X (1+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 1 3 4 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 1 3 4 6 or 7.
 * https://oeis.org/A252236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-3) - 32*a(n-6) (n > 8) Initial values: a(1) = 342,
 *            a(2) = 858, a(3) = 2174, a(4) = 4708, a(5) = 13376, a(6) =
 *            34364, a(7) = 74824, a(8) = 213344 Offset: 1 (a(1) is the first
 *            term)
 * Range:     u8 none, u16 n <= 6, u32 n <= 18, u64 n <= 42, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cache[1..n] of sequence values   */
static idx_t cache_max_n = 0;         /* largest n computed so far       */
static const seq_t base_vals[9] = {
    0,          /* dummy for index 0 */
    342, 858, 2174, 4708, 13376, 34364, 74824, 213344
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety guard, sequence defined for n>=1 */

    /* enlarge cache if needed */
    if (n > cache_max_n) {
        size_t new_cap = (size_t)n + 1;   /* index n is reachable */
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();           /* allocation failure */
        cache = new_ptr;

        /* compute new entries */
        for (idx_t i = cache_max_n + 1; i <= n; ++i) {
            if (i <= 8) {
                cache[i] = base_vals[i];
            } else {
                /* recurrence: 18*a(i-3) - 32*a(i-6) */
                cache[i] = ((seq_t)18) * cache[i-3] - ((seq_t)32) * cache[i-6];
            }
        }
        cache_max_n = n;
    }
    return cache[n];
}
