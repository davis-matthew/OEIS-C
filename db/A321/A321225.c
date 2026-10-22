/*
 * A321225  Break the infinite word 1232112321... into chunks so that the sum
 *          of the digits in the n-th chunk is n.
 * https://oeis.org/A321225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 100001*a(n-9) - 100000*a(n-18) for n>18 Initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=211, a(5)=23, a(6)=2112, a(7)=3211,
 *            a(8)=2321, a(9)=12321, a(10)=123211, a(11)=232112, a(12)=321123,
 *            a(13)=21123211,a(14)=2321123, a(15)=211232112,
 *            a(16)=321123211,a(17)=232112321,a(18)=1232112321
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   compute_chunk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_chunk.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array a[1..*] (index 0 unused) */
    static size_t cache_len = 0;  /* largest index already computed */

    if (n == 0) return 0;         /* safety; not used by OEIS */

    /* extend cache if necessary */
    if (n > cache_len) {
        size_t new_cap = n + 1;    /* +1 to use 1‑based indexing */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        /* compute missing terms */
        for (size_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 18) {
                cache[i] = compute_chunk((idx_t)i);
            } else {
                cache[i] = cache[i - 9] * (seq_t)100001
                           - cache[i - 18] * (seq_t)100000;
            }
        }
        cache_len = n;
    }
    return cache[n];
}
