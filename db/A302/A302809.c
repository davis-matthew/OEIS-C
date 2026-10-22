/*
 * A302809  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 14*a(n-2) + 2*a(n-3) - 22*a(n-4) + 3*a(n-5) +
 *            37*a(n-6) - 19*a(n-7) - 4*a(n-8) + 24*a(n-9) - 4*a(n-10) +
 *            11*a(n-11) + 8*a(n-12) + 2*a(n-13) for n > 15. Initial values:
 *            a(1)=4, a(2)=32, a(3)=169, a(4)=934, a(5)=5117, a(6)=28128,
 *            a(7)=154494, a(8)=848519, a(9)=4660562, a(10)=25597...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* Initialise cache and first 15 terms once */
    if (cache == NULL) {
        /* Reserve space for n up to at least 10000 */
        cache_cap = 10001;
        cache = alloc_seq_array(cache_cap);

        /* first term unused, index 1..15 initialised */
        static const seq_t init[16] = {
            (seq_t)0ULL,
            (seq_t)4ULL,
            (seq_t)32ULL,
            (seq_t)169ULL,
            (seq_t)934ULL,
            (seq_t)5117ULL,
            (seq_t)28128ULL,
            (seq_t)154494ULL,
            (seq_t)848519ULL,
            (seq_t)4660562ULL,
            (seq_t)25597855ULL,
            (seq_t)140595667ULL,
            (seq_t)772217905ULL,
            (seq_t)4241385457ULL,
            (seq_t)23295695745ULL,
            (seq_t)127950975583ULL
        };
        for (idx_t i = 1; i <= 15 && i < cache_cap; ++i)
            cache[i] = init[i];

        max_computed = 15;
    }

    /* Grow cache if necessary (for n > 10000) */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (idx_t i = 0; i < max_computed; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 15) continue;          /* already initialised */
        seq_t val = 0;
        val += 3   * cache[i-1];
        val += 14  * cache[i-2];
        val += 2   * cache[i-3];
        val -= 22  * cache[i-4];
        val += 3   * cache[i-5];
        val += 37  * cache[i-6];
        val -= 19  * cache[i-7];
        val -= 4   * cache[i-8];
        val += 24  * cache[i-9];
        val -= 4   * cache[i-10];
        val += 11  * cache[i-11];
        val += 8   * cache[i-12];
        val += 2   * cache[i-13];
        cache[i] = val;
    }

    if (n > max_computed)
        max_computed = n;

    return cache[n];
}
