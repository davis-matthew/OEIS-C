/*
 * A337724  a(n) = prime(n-2) - floor(a(n-2)/2); a(1)=0, a(2)=1.
 * https://oeis.org/A337724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n‑2) – floor(a(n‑2)/2) with a(1)=0, a(2)=1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   get_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/get_prime.h"     /* get_prime(idx_t) returns seq_t */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*,size_t,size_t) */

seq_t a(idx_t n)
{
    /* cache[i] stores a(i+1)  (0‑based index) */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  max_computed = 0;

    if (n == 0) return 0;      /* offset is 1, but allow n==0 safely */

    /* ensure cache has room for n entries */
    if ((size_t)n > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)n);
        cache_cap = (size_t)n;
    }

    /* compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 0;
        } else if (i == 2) {
            val = 1;
        } else {
            /* a(i) = prime(i‑2) – floor(a(i‑2)/2) */
            seq_t a_i_2 = cache[(size_t)i - 3];          /* a(i‑2) */
            val = get_prime(i - 2) - (a_i_2 >> 1);   /* floor division by 2 */
        }
        cache[(size_t)i - 1] = val;
    }
    if (n > max_computed) max_computed = n;

    return cache[(size_t)n - 1];
}
