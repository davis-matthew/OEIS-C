/*
 * A378784  a(1)=1, a(2)=2, then the terms from a(2^k + 1) to a(2^(k+1)) are
 *          the first 2^k terms in reverse order, increased by a(2^k).
 * https://oeis.org/A378784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2. For n>2 let m be the largest power of two less
 *            than n. Then a(n)=a(2*m-n+1)+a(m). This follows from the
 *            definition that a(m+1)…a(2m) are the first m terms in reverse
 *            order, each increased by a(m)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;        /* array 1..computed_size */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t computed_size = 0;    /* last index computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n<1 */

    if (n > computed_size) {
        /* ensure a buffer large enough */
        size_t required = (size_t)n + 1; /* indices 0..n */
        if (required > cache_cap) {
            seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
            if (cache) {
                for (size_t i = 1; i <= computed_size; ++i)
                    new_cache[i] = cache[i];
                free(cache);
            }
            cache = new_cache;
            cache_cap = required;
        }

        /* initialise the first two terms */
        cache[1] = 1;
        if (n >= 2) cache[2] = 2;

        /* generate all terms up to n */
        idx_t m = 2;
        while (m < n) {
            idx_t next = m << 1;        /* 2*m */
            if (next > n) next = n;
            seq_t base = cache[m];
            for (idx_t j = 1; j <= next - m; ++j) {
                idx_t idx = m + j;
                cache[idx] = cache[m - j + 1] + base;
            }
            m = next;
        }
        computed_size = n;
    }
    return cache[n];
}
