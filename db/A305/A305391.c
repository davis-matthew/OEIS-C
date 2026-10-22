/*
 * A305391  A ternary tribonacci sequence: define the morphism f: 1 -> 2, 2 ->
 *          3, 3 -> 1,2,3; let S[k] be result of applying f k times to 1, for
 *          k =- 0,1,2,...; sequence gives limit S[3k+2] as k -> oo.
 * https://oeis.org/A305391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th symbol of the infinite word obtained by
 *            repeatedly applying the morphism f: 1 → 2, 2 → 3, 3 → 1,2,3,
 *            starting with 1 and taking the limit of S[3k+2] (k → ∞). The
 *            word begins 3,1,2,3,2,3,1,2,3,1,2,3,2,3,1,2,3,3,1,2,3,2,3,1,
 * Range:     u8 n <= 9336, u16 n <= 9336, u32 n <= 9336, u64 n <= 9336,
 *            u128 n <= 9336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *prefix = NULL;
static size_t prefix_len = 0;

seq_t a(idx_t n) {
    /* initialise the first prefix S[2] = "3" */
    if (prefix == NULL) {
        prefix = (seq_t*)malloc(1 * sizeof(seq_t));
        prefix[0] = (seq_t)3;
        prefix_len = 1;
    }

    /* extend until the word contains position n */
    while ((size_t)n >= prefix_len) {
        /* apply the morphism f three times to get S[3k+5] from S[3k+2] */
        for (int step = 0; step < 3; ++step) {
            /* compute the length of the new word */
            size_t new_len = 0;
            for (size_t i = 0; i < prefix_len; ++i) {
                seq_t v = prefix[i];
                if (v == (seq_t)1 || v == (seq_t)2)
                    new_len += 1;
                else /* v == 3 */
                    new_len += 3;
            }

            /* allocate and fill the new word */
            seq_t *new_prefix = (seq_t*)malloc(new_len * sizeof(seq_t));
            size_t pos = 0;
            for (size_t i = 0; i < prefix_len; ++i) {
                seq_t v = prefix[i];
                if (v == (seq_t)1) {
                    new_prefix[pos++] = (seq_t)2;
                } else if (v == (seq_t)2) {
                    new_prefix[pos++] = (seq_t)3;
                } else { /* v == 3 */
                    new_prefix[pos++] = (seq_t)1;
                    new_prefix[pos++] = (seq_t)2;
                    new_prefix[pos++] = (seq_t)3;
                }
            }

            /* replace old word */
            free(prefix);
            prefix = new_prefix;
            prefix_len = new_len;
        }
    }

    return prefix[(size_t)n];
}
