/*
 * A391096  a(n) is largest number such that the sequence contains a total of
 *          a(n) base 10 digits + commas through n terms (assuming one comma
 *          between each pair of terms).
 * https://oeis.org/A391096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1; for n>1 let * T = sum_{i=1}^{n-1} digits(a(i)) + (n-1).
 *            * a(n) is the largest integer k such that k - digits(k) = T. *
 *            The sequence increases by (digits+1) each step after each
 *            threshold
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms (1‑based) */
    static seq_t *cache = NULL;        /* array of terms */
    static idx_t cache_len = 0;        /* highest index cached */
    static size_t cache_cap = 0;       /* current array capacity   */
    static seq_t sum_digits = 0;       /* cumulative sum of digits */

    if (n == 0) return 0;              /* safety for 0‑based callers */

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t base = sum_digits + (i - 1);   /* T in the formula */

        seq_t best = 0;
        /* search for the largest k.  The search bound is generous. */
        for (int d = 1; d <= 128; ++d) {
            seq_t cand = base + (seq_t)d;
            if (digit_len(cand) == d) {
                best = cand;                 /* keep the largest found */
            }
        }
        /* best must be found; the problem guarantees a solution. */
        cache[i] = best;
        sum_digits += (seq_t)digit_len(best); /* update cumulative digits */
    }
    cache_len = n;
    return cache[n];
}
