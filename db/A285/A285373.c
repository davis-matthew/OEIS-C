/*
 * A285373  Fixed point of the morphism 0 -> 10, 1 -> 1110.
 * https://oeis.org/A285373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the infinite fixed point word of the
 *            morphism 0 -> 10, 1 -> 1110. The implementation generates the
 *            word incrementally starting from the symbol '1', expanding until
 *            it is long enough to contain the requested position. It works
 *            for any seq_t type (plain integer or arbitrar...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
static inline seq_t *alloc_seq(size_t n) {
    return new seq_t[n];
}
static inline void free_seq(seq_t *p) {
    delete[] p;
}
#else
static inline seq_t *alloc_seq(size_t n) {
    return (seq_t *)malloc(n * sizeof(seq_t));
}
static inline void free_seq(seq_t *p) {
    free(p);
}
#endif

seq_t a(idx_t n) {
    if (n == 0) {          /* offset >= 1, but zero is illegal */
        return (seq_t)0;
    }

    /* Use a size_t for array indexing; ns up to ~10000 in the tests. */
    size_t nn = (size_t)n;

    /* Start from the seed symbol '1', which is a prefix of the fixed point. */
    seq_t *cur = alloc_seq(1);
    cur[0] = (seq_t)1;
    size_t cur_len = 1;

    /* Expand until the word is long enough to contain the desired index. */
    while (cur_len < nn) {
        size_t i, pos;
        /* Compute the length of the next expansion. */
        size_t next_len = 0;
        for (i = 0; i < cur_len; ++i) {
            next_len += (cur[i] == (seq_t)1) ? 4U : 2U;
        }

        seq_t *next = alloc_seq(next_len);
        pos = 0;
        for (i = 0; i < cur_len; ++i) {
            if (cur[i] == (seq_t)1) {
                /* 1 -> 1110 */
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
            } else {
                /* 0 -> 10 */
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
            }
        }

        free_seq(cur);
        cur = next;
        cur_len = next_len;
    }

    seq_t result = cur[nn - 1];
    free_seq(cur);
    return result;
}
