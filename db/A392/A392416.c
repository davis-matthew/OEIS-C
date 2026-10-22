/*
 * A392416  Number of partitions of the vertices of the n X 3 grid graph into
 *          total dominating sets.
 * https://oeis.org/A392416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392416
   Recurrence (n ≥ 11):
     a(n) = 6*a(n-1) - 5*a(n-2) - 13*a(n-3) + 12*a(n-4)
            + 45*a(n-5) - 70*a(n-6) + 26*a(n-7) + 12*a(n-8)
            - 20*a(n-9) + 8*a(n-10)
   Initial values (index 1‑based):
     a(1)=1, a(2)=3, a(3)=15, a(4)=33, a(5)=187,
     a(6)=723, a(7)=3265, a(8)=14451, a(9)=63707, a(10)=282753
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset trivially */
    if (n == 0) return (seq_t)0;

    /* Base values for indices 1..10 */
    static const seq_t base[11] = {
        0,                /* unused index 0 */
        1,
        3,
        15,
        33,
        187,
        723,
        3265,
        14451,
        63707,
        282753
    };

    /* For small n return directly */
    if (n <= 10) return base[n];

    /* Small static cache that grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* allocated size */
    static idx_t last_computed = 0;  /* largest index with a value */

    /* Ensure the cache can hold indices up to n */
    if (cache_size < n + 1U) {
        idx_t new_size = n + 1U;
        seq_t *new_cache = NULL;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (new_cache == NULL) abort();          /* allocation failed */
        cache        = new_cache;
        cache_size   = new_size;
    }

    /* Initialise base values in the cache if not already done */
    if (last_computed < 10) {
        for (idx_t i = 1; i <= 10; ++i) {
            cache[i] = base[i];
        }
        last_computed = 10;
    }

    /* Compute terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val  = 6 * cache[i - 1];
        val -= 5 * cache[i - 2];
        val -= 13 * cache[i - 3];
        val += 12 * cache[i - 4];
        val += 45 * cache[i - 5];
        val -= 70 * cache[i - 6];
        val += 26 * cache[i - 7];
        val += 12 * cache[i - 8];
        val -= 20 * cache[i - 9];
        val += 8 * cache[i - 10];
        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
