/*
 * A337643  a(1) = 1; thereafter a(n) = a(n-1) / lpf(n) if lpf(n) divides
 *          a(n-1), otherwise a(n) = a(n-1) * lpf(n), where lpf is the least
 *          prime factor function A020639.
 * https://oeis.org/A337643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A337643:
   a(1) = 1;
   for n > 1:
       let p = lpf(n‑1);      // least prime factor of n‑1
       if a(n‑1) % p == 0
           a(n) = a(n‑1) / p
       else
           a(n) = a(n‑1) * p
*/
#include <stdlib.h>
#include "../utils/lpf.h"

static seq_t *cache = NULL;          /* cache[0] unused, cache[1] = a(1) */
static idx_t cache_len = 0;          /* highest n computed */
static size_t cache_cap = 0;         /* allocated capacity */

/* Return a(n) for n >= 1.  Undefined for n == 0 (returns 0). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;      /* allocation failure */
        cache[0] = (seq_t)0;       /* unused slot for index 0 */
        cache[1] = (seq_t)1;       /* a(1) = 1 */
        cache_len = 1;
    }

    /* Grow cache if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 2);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        idx_t l = lpf(k - 1);          /* least prime factor of k-1 */
        seq_t prev = cache[k - 1];
        if (prev % (seq_t)l == 0)
            cache[k] = prev / (seq_t)l; /* division case */
        else
            cache[k] = prev * (seq_t)l; /* multiplication case */
    }
    cache_len = n;

    return cache[n];
}
