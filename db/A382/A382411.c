/*
 * A382411  a(n) is the greatest possible length of a circular sequence on n
 *          symbols such that: no two adjacent symbols are the same, any group
 *          of n adjacent symbols contains at least n-1 different symbols, and
 *          all groups of n adjacent symbols within the sequence are unique.
 * https://oeis.org/A382411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (n^2 - 3*n + 4) / 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A382411.
 * n >= 1; for n == 0 the function returns 0 (behaviour below
 * the offset is irrelevant). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* ---- factorial cache ---- */
    /* static cache: fact_cache[i] = i!  (0 <= i <= cache_size) */
    static seq_t *fact_cache = NULL;
    static idx_t cache_size = 0;

    if (n > cache_size) {
        /* allocate or reallocate larger cache */
        idx_t new_size = n + 1;  /* include index n */
        fact_cache = (seq_t *)realloc(fact_cache,
                                      new_size * sizeof(seq_t));

        /* initialise new entries */
        if (cache_size == 0) {
            fact_cache[0] = (seq_t)1;   /* 0! = 1 */
        }
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        cache_size = n;
    }

    /* factorial of n */
    seq_t fact = fact_cache[n];

    /* polynomial part: n^2 - 3*n + 4 */
    seq_t nn = (seq_t)n;
    seq_t poly = nn * nn - (seq_t)3 * nn + (seq_t)4;

    /* combine and divide by 2 */
    seq_t result = fact * poly;
    result /= 2;

    return result;
}
