/*
 * A287819  Number of nonary sequences of length n such that no two
 *          consecutive terms have distance 4.
 * https://oeis.org/A287819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-2) - 14*a(n-3) for n>=4, * with initial
 *            values a(0)=1, a(1)=9, a(2)=71, a(3)=561
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of already computed values */
    static seq_t *seq = NULL;
    static idx_t  seq_len = 0;   /* number of elements ready  */
    static size_t seq_cap = 0;   /* allocated capacity          */

    /* Grow the cache to accommodate index n */
    if (seq_cap <= (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* Constants used in the recurrence */
    const seq_t eight   = (seq_t)8;
    const seq_t fourteen= (seq_t)14;

    /* Compute values up to n */
    for (idx_t i = seq_len; i <= n; ++i) {
        size_t ui = (size_t)i;
        if (i == 0)          seq[ui] = 1;
        else if (i == 1)     seq[ui] = 9;
        else if (i == 2)     seq[ui] = 71;
        else if (i == 3)     seq[ui] = 561;
        else                 seq[ui] = eight * seq[ui-1] + seq[ui-2]
                                      - fourteen * seq[ui-3];
        seq_len = i + 1;
    }

    return seq[(size_t)n];
}
