/*
 * A289235  Fixed point of the mapping 00->0010, 01->100, 10->000, starting
 *          with 00.
 * https://oeis.org/A289235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th bit of the fixed point of the block substitution
 * mapping 00 → 0010, 01 → 100, 10 → 000, starting from the word "00".
 * The function generates the word up to the requested index and returns the
 * corresponding bit as a seq_t value.
 */
#include <stdlib.h>

#ifdef __cplusplus
#define ALLOC_SEQ(n) new seq_t[(size_t)(n)]
#define FREE_SEQ(p) delete[] (p)
#else
#define ALLOC_SEQ(n) ((seq_t*)malloc((size_t)(n)*sizeof(seq_t)))
#define FREE_SEQ(p) free((p))
#endif

seq_t a(idx_t n) {
    /* n is 1‑based (n ≥ 1) */
    if (n == 0) return (seq_t)0; /* safety, not used in OEIS offset */

    static seq_t *cache = NULL;   /* previously computed word          */
    static size_t clen   = 0;     /* length of the cached word         */

    /* use cached value if it already contains the desired index */
    if ((size_t)n <= clen) {
        return cache[(size_t)(n - 1)];
    }

    /* we need to generate the word up to at least index n */
    size_t target = (size_t)n;
    size_t cur_len = 2;
    seq_t *cur = ALLOC_SEQ(2);
    cur[0] = (seq_t)0;
    cur[1] = (seq_t)0;

    /* iterate until the current word is long enough */
    while (cur_len < target) {
        size_t pairs = cur_len >> 1;
        size_t next_est = pairs * 4 + (cur_len & 1);
        seq_t *next = ALLOC_SEQ(next_est);
        size_t pos = 0;

        for (size_t i = 0; i + 1 < cur_len; i += 2) {
            seq_t a1 = cur[i];
            seq_t a2 = cur[i + 1];

            if (a1 == (seq_t)0 && a2 == (seq_t)0) {        /* 00 → 0010 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
            } else if (a1 == (seq_t)0 && a2 == (seq_t)1) {   /* 01 → 100 */
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
            } else { /* a1 == 1 && a2 == 0  →  10 → 000 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
            }
        }
        if (cur_len & 1) { /* carry the last symbol if length is odd */
            next[pos++] = cur[cur_len - 1];
        }

        FREE_SEQ(cur);          /* free previous word */
        cur = next;             /* move to the next word */
        cur_len = pos;
    }

    /* replace the cache with the new word */
    FREE_SEQ(cache);
    cache = cur;
    clen = cur_len;

    return cache[(size_t)(n - 1)];
}
