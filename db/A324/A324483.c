/*
 * A324483  Expansion of
 *          (1-x-x^2)^2*(1+x-x^2)^2/((1-3*x+x^2)*(1-x)^2*(1+x)^4).
 * https://oeis.org/A324483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A324483:
   Recurrence: a(n) = a(n-1) + 6*a(n-2) - a(n-3) - 10*a(n-4) - a(n-5) + 6*a(n-6) + a(n-7) - a(n-8)
   Initial terms: a(0)=1, a(1)=1, a(2)=1, a(3)=6, a(4)=12, a(5)=36, a(6)=91, a(7)=241, a(8)=632
*/

#include "../utils/seq_alloc.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    seq_t *new_cache = seq_alloc(new_size);

    /* copy old data if any */
    for (idx_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];

    cache = new_cache;
    cache_size = new_size;

    /* initialise base values (if not already) */
    if (cache_size > 0) { cache[0] = 1; }
    if (cache_size > 1) { cache[1] = 1; }
    if (cache_size > 2) { cache[2] = 1; }
    if (cache_size > 3) { cache[3] = 6; }
    if (cache_size > 4) { cache[4] = 12; }
    if (cache_size > 5) { cache[5] = 36; }
    if (cache_size > 6) { cache[6] = 91; }
    if (cache_size > 7) { cache[7] = 241; }
    if (cache_size > 8) { cache[8] = 632; }

    /* compute missing values using the recurrence */
    idx_t start = old_size > 8 ? old_size : 9;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = cache[i - 1] + (seq_t)6 * cache[i - 2];
        val -= cache[i - 3];
        val -= (seq_t)10 * cache[i - 4];
        val -= cache[i - 5];
        val += (seq_t)6 * cache[i - 6];
        val += cache[i - 7];
        val -= cache[i - 8];
        cache[i] = val;
    }
    return cache[n];
}
