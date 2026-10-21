/*
 * A270106  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 84", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1..n} 2^{popcount(k)+⌊log₂(k)⌋+1} (where
 *            popcount(k) is the number of 1‑bits in k, and ⌊log₂(0)⌋ is
 *            defined only for k>0). This yields the partial sums of the
 *            active cells in Rule‑84 von Neumann CA
 * Range:     u8 n <= 8, u16 n <= 84, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, exit */

/* Return the nth term of A270106.  */
seq_t a(idx_t n)
{
    /* static cache of partial sums: cache[i] = a(i) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached terms (== cache_len) */

    /* grow cache if necessary */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);   /* allocation failed */
        cache = new_cache;

        /* start sum from previous maximum (if any) */
        seq_t sum = (cache_len > 0) ? cache[cache_len - 1] : 0;

        /* compute terms from cache_len up to n */
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t f_val;
            if (k == 0) {
                f_val = (seq_t)1;   /* f(0) = 1 */
            } else {
                /* popcount(k) and floorlog2(k) using GCC/Clang builtins */
                unsigned int pop = __builtin_popcountll((unsigned long long)k);
                unsigned int flr = 63 - __builtin_clzll((unsigned long long)k);
                unsigned int exp = pop + flr + 1;   /* exponent for 2^exp */

                /* compute 2^exp modulo the width of seq_t by repeated shifts */
                seq_t val = (seq_t)1;
                for (unsigned int i = 0; i < exp; ++i)
                    val <<= 1;
                f_val = val;
            }

            sum += f_val;        /* a(k) = a(k-1) + f(k) */
            cache[k] = sum;
        }

        cache_len = new_len;
    }

    return cache[n];
}
