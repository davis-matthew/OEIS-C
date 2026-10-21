/*
 * A242147  Number of length 3+5 0..n arrays with no consecutive six elements
 *          summing to more than 3*n.
 * https://oeis.org/A242147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n > 9
 *            Initial values: a(1)=132; a(2)=2902; a(3)=27024; a(4)=154647;
 *            a(5)=647404; a(6)=2180310; a(7)=6256170; a(8)=15876783;
 *            a(9)=36560854;
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 17, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;      // current allocated capacity (index 0 unused)
    static size_t filled = 0;   // highest index initialized (>= 0)

    /* Ensure at least indices 1..9 are available */
    if (filled < 9) {
        if (cap < 10) {
            cap = 16;               /* initial capacity larger than 9 */
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            cache = tmp;
        }
        /* initialize base values */
        cache[1] = 132;
        cache[2] = 2902;
        cache[3] = 27024;
        cache[4] = 154647;
        cache[5] = 647404;
        cache[6] = 2180310;
        cache[7] = 6256170;
        cache[8] = 15876783;
        cache[9] = 36560854;
        filled = 9;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Compute values up to n if not already done */
    for (idx_t i = (idx_t)(filled + 1); i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)9   * cache[i - 1];
        val -= (seq_t)36  * cache[i - 2];
        val += (seq_t)84  * cache[i - 3];
        val -= (seq_t)126 * cache[i - 4];
        val += (seq_t)126 * cache[i - 5];
        val -= (seq_t)84  * cache[i - 6];
        val += (seq_t)36  * cache[i - 7];
        val -= (seq_t)9   * cache[i - 8];
        val +=           cache[i - 9];
        cache[i] = val;
    }
    filled = (size_t)n;

    return cache[n];
}
