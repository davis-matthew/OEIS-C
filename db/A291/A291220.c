/*
 * A291220  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^4.
 * https://oeis.org/A291220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 5*a(n-4) * + 3*a(n-5) +
 *            4*a(n-6) - a(n-7) - a(n-8) * Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=7, * a(5)=15, a(6)=27, a(7)=55, a(8)=101
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 69, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t stored = 0;

    /* Initial terms for n = 0 … 8 */
    const seq_t init[9] = {1, 1, 2, 4, 7, 15, 27, 55, 101};

    /* First call: allocate and store the first nine terms */
    if (stored == 0) {
        size_t init_cap = 16;
        while (init_cap < 9) init_cap <<= 1;
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory, return 0 as sentinel */
        cap = init_cap;
        for (size_t i = 0; i < 9; ++i) cache[i] = init[i];
        stored = 9;
    }

    /* Expand cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* If n is already computed, return it */
    if (n < stored) return cache[(size_t)n];

    /* Compute new terms up to n */
    for (size_t i = stored; i <= (size_t)n; ++i) {
        /* i >= 9 due to stored >= 9 and n >= stored */
        seq_t val = cache[i-1];
        val += 4 * cache[i-2];
        val -= 3 * cache[i-3];
        val -= 5 * cache[i-4];
        val += 3 * cache[i-5];
        val += 4 * cache[i-6];
        val -= cache[i-7];
        val -= cache[i-8];
        cache[i] = val;
    }
    stored = need;
    return cache[(size_t)n];
}
