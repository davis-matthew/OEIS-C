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
 * Validated: standard, pass, 87 terms through n = 9998 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = n                                       \
 * If n is the k-th element of the subsequence F of numbers   \
 * divisible by 7 or containing a digit 7, then a(n) = a(k).   */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline bool in_F(idx_t x)
{
    if (x % 7 == 0) return true;
    while (x > 0) {
        if (x % 10 == 7) return true;
        x /= 10;
    }
    return false;
}

/* Cache of already computed a values; cache[0] is unused. */
static seq_t *cache       = NULL;
static idx_t   max_computed = 0;
static idx_t   f_count      = 0;   /* number of elements of F seen so far */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n < 1 */

    if (n > max_computed) {
        idx_t new_max = n;
        seq_t *tmp = mem_resize(seq_t, cache, (new_max + 1));
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
