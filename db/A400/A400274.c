/*
 * A400274  G.f. A(x) satisfies A(x) = (1 + x) * A(x^4 * (1 + x)^4), with A(0)
 *          = 1.
 * https://oeis.org/A400274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A400274
   Recurrence: a(0) = 1;
   a(n) = Σ_{k=0..⌊n/4⌋} binomial(4k+1, n-4k) * a(k)
          (where binomial is provided by the build)
*/

#include <stdlib.h>
#include "../utils/binom.h"   /* provides: seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i < filled. */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated size of cache   */
    static size_t  filled = 0;   /* number of entries computed */

    /* Allocate / grow cache if necessary (exponential growth). */
    if (n >= (idx_t)cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* memory exhausted */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = (idx_t)filled; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;                /* a(0) = 1 */
            continue;
        }
        seq_t sum = 0;
        idx_t maxk = i / 4;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t t = i - 4 * k;         /* n - 4k  */
            seq_t c = binom(4 * k + 1, t);
            sum += c * cache[(size_t)k];
        }
        cache[(size_t)i] = sum;
    }

    filled = (size_t)(n + 1);
    return cache[(size_t)n];
}
