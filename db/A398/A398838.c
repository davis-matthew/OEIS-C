/*
 * A398838  Lower (1, 4/3)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2, a(2)=9, a(3)=22, a(4)=39, a(n) = (15*n^2 - 31*n
 *            + 24 - 2*a(n-1) - 2*a(n-2)) / 2 for n ≥ 2
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values for n < 5. */
    static const seq_t init[5] = {0, 2, 9, 22, 39};

    if (n < 5) return init[n];

    static seq_t *cache = NULL;
    static size_t size = 0, cap = 0;

    /* Initialize cache with first five terms on first call. */
    if (cache == NULL) {
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        size = 5;
    }

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cap) {
        cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Compute terms up to n if not already computed. */
    for (size_t i = size; i <= (size_t)n; ++i) {
        if (i < 5) {
            cache[i] = init[i];
            continue;
        }
        seq_t i_seq = (seq_t)i;
        seq_t term = (seq_t)15 * i_seq * i_seq
                   - (seq_t)31 * i_seq
                   + (seq_t)24;
        seq_t res = (term
                   - (seq_t)2 * cache[i - 1]
                   - (seq_t)2 * cache[i - 2]) / (seq_t)2;
        cache[i] = res;
    }
    size = (size_t)n + 1;
    return cache[(size_t)n];
}
