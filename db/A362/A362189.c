/*
 * A362189  Lexicographically earliest sequence of positive integers having
 *          the same concatenation of digits as the sequence 2^a(n).
 * https://oeis.org/A362189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   id_of_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the fixed point of the morphism
   1 → 2, 2 → 4, 4 → 16, 6 → 64 starting from 6.
   This function returns the n‑th term (0‑based) of A362189. */

#include <stdlib.h>
#include "../utils/id_of_digit.h"

static const int map_len[4] = {1, 1, 2, 2};
static const int map_digits[4][2] = {
    {2},     /* 1 → 2 */
    {4},     /* 2 → 4 */
    {1, 6},  /* 4 → 16 */
    {6, 4}   /* 6 → 64 */
};
static const int digit_of_id[4] = {1, 2, 4, 6};

seq_t a(idx_t n) {
    /* Find the minimum depth such that the expansion of 6 has length > n. */
    size_t depth = 0;
    /* len_depth[id] will hold the length of the expansion of each letter at
       the current depth. Start with depth 0: length 1 for every letter. */
    idx_t len_depth[4] = {1, 1, 1, 1};

    /* Keep increasing depth until the length of the expansion of 6 is large enough. */
    while (len_depth[3] <= n) {
        idx_t next[4];
        for (int id = 0; id < 4; ++id) {
            idx_t sum = 0;
            int mlen = map_len[id];
            for (int k = 0; k < mlen; ++k) {
                int dig = map_digits[id][k];
                int did = id_of_digit(dig);
                sum += len_depth[did];
            }
            next[id] = sum;
        }
        for (int id = 0; id < 4; ++id) len_depth[id] = next[id];
        depth++;
    }

    /* Allocate a table of lengths for all depths from 0 to depth. */
    idx_t (*len)[4] = (idx_t (*)[4])malloc((depth + 1) * sizeof(idx_t[4]));
    if (!len) return 0;           /* allocation failure → return 0 */
    for (int id = 0; id < 4; ++id) len[0][id] = 1;     /* depth 0 */

    for (size_t d = 1; d <= depth; ++d) {
        for (int id = 0; id < 4; ++id) {
            idx_t sum = 0;
            int mlen = map_len[id];
            for (int k = 0; k < mlen; ++k) {
                int dig = map_digits[id][k];
                int did = id_of_digit(dig);
                sum += len[d - 1][did];
            }
            len[d][id] = sum;
        }
    }

    /* Walk down from the deepest expansion to level 0. */
    size_t cur_depth = depth;
    int cur_id = 3;          /* start from the symbol '6' */
    idx_t pos = n;

    while (cur_depth > 0) {
        int mlen = map_len[cur_id];
        for (int k = 0; k < mlen; ++k) {
            int dig = map_digits[cur_id][k];
            int did = id_of_digit(dig);
            idx_t sublen = len[cur_depth - 1][did];
            if (pos < sublen) {
                cur_id = did;
                cur_depth--;
                break;
            } else {
                pos -= sublen;
            }
        }
    }

    /* At depth 0, the current symbol itself is the answer. */
    seq_t result = (seq_t)digit_of_id[cur_id];
    free(len);
    return result;
}
