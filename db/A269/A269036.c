/*
 * A269036  Number of 2 X n 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) - 4*a(n-4) (n ≥ 5), with initial
 *            values a(1)=0, a(2)=24, a(3)=48, a(4)=216
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache holding already computed values */
    /* Size 100001 is sufficient for n up to 100000; the sequence is
       required only up to about 10000.  Adjust if a larger range is
       necessary. */
    static seq_t cache[100001];
    static bool initialized = false;
    static idx_t max_computed = 0;

    if (n == 0)
        return 0;           /* undefined, but safe */

    if (!initialized) {
        /* set known initial values */
        cache[1] = 0;
        cache[2] = 24;
        cache[3] = 48;
        cache[4] = 216;
        max_computed = 4;
        initialized = true;
    }

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i >= 5) {
            cache[i] = (seq_t)4 * cache[i - 1]
                     - (seq_t)8 * cache[i - 3]
                     - (seq_t)4 * cache[i - 4];
        } else {
            /* i == 1..4 already set */
            /* nothing to do */
            (void)i;
        }
    }
    max_computed = (n > max_computed) ? n : max_computed;
    return cache[n];
}
