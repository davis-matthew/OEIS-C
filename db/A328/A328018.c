/*
 * A328018  If n is the k-th number divisible by 7 or containing a digit 7 (in
 *          base 10) then a(n) = a(k) otherwise a(n) = n.
 * https://oeis.org/A328018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_F
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = n                                       \
 * If n is the k-th element of the subsequence F of numbers   \
 * divisible by 7 or containing a digit 7, then a(n) = a(k).   */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/in_F.h"

/* Cache of already computed a values; cache[0] is unused. */
static seq_t *cache       = NULL;
static idx_t   max_computed = 0;
static idx_t   f_count      = 0;   /* number of elements of F seen so far */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n < 1 */

    if (n > max_computed) {
        idx_t new_max = n;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (new_max + 1) * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        cache = tmp;

        for (idx_t i = max_computed + 1; i <= new_max; ++i) {
            if (in_F(i)) {
                ++f_count;                 /* i is the f_count‑th element of F */
                cache[i] = cache[f_count]; /* a(i) = a(f_count) */
            } else {
                cache[i] = (seq_t)i;        /* non‑F numbers keep their value */
            }
        }
        max_computed = new_max;
    }
    return cache[n];
}
