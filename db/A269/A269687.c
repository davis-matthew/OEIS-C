/*
 * A269687  Number of length-n 0..5 arrays with no repeated value differing
 *          from the previous repeated value by plus or minus one modulo 5+1.
 * https://oeis.org/A269687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 21*a(n-2) - 20*a(n-3) with a(1)=6, a(2)=36,
 *            a(3)=216
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed = 0; /* highest index computed */

    if (n == 0) return 0; /* undefined for n<1, return 0 */

    /* Ensure cache large enough */
    if (n >= cache_cap) {
        /* Allocate at least n+1 (to allow 1‑based indexing) */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    while (computed < n) {
        ++computed;
        if (computed == 1)
            cache[1] = (seq_t)6;
        else if (computed == 2)
            cache[2] = (seq_t)36;
        else if (computed == 3)
            cache[3] = (seq_t)216;
        else
            cache[computed] = (seq_t)10 * cache[computed-1] -
                              (seq_t)21 * cache[computed-2] -
                              (seq_t)20 * cache[computed-3];
    }

    return cache[n];
}
