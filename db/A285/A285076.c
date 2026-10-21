/*
 * A285076  1-limiting word of the morphism 0->10, 1-> 010.
 * https://oeis.org/A285076
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

/* OEIS A285076 – 1-limiting word of the morphism 0->10, 1->010
 *
 *  The word is obtained by iterating the morphism
 *       0 -> 10
 *       1 -> 010
 *  starting from the single symbol 0 and stopping at the first odd number
 *  of iterations that produces a word of length ≥ n.  The nth term is
 *  the (n‑1)th symbol of that word.
 *
 *  Complexity:  O(L) where L is the length of the generated word
 *  (about 2.4^k for k iterations; < 11000 for n ≤ 10000).
 *
 *  The function uses only integer arithmetic, complies with C11
 *  and C++17, and respects the required types seq_t and idx_t. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Starting word: "0" */
    size_t cur_len = 1;
    seq_t *cur = (seq_t *)calloc(cur_len, sizeof(seq_t));
    if (!cur) return 0;            // safety, though not expected
    cur[0] = 0;

    idx_t iter = 0;                // number of applied morphism iterations

    for (;;)
    {
        /* If we are at an odd iteration and the word is long enough,
           return the desired nth element. */
        if ((iter & 1) && cur_len >= n)
        {
            seq_t result = cur[(size_t)(n - 1)];
            free(cur);
            return result;
        }

        /* Count zeros and ones to preallocate next word. */
        size_t zeros = 0, ones = 0;
        for (size_t i = 0; i < cur_len; ++i)
            (cur[i] == 0) ? ++zeros : ++ones;

        /* Length of the next word: each 0 -> 2 chars, each 1 -> 3 chars. */
        size_t next_len = zeros * 2 + ones * 3;
        seq_t *next_arr = (seq_t *)calloc(next_len, sizeof(seq_t));
        if (!next_arr) { free(cur); return 0; }

        /* Apply the morphism. */
        size_t pos = 0;
        for (size_t i = 0; i < cur_len; ++i)
        {
            if (cur[i] == 0)
            {
                next_arr[pos++] = 1;
                next_arr[pos++] = 0;
            }
            else
            {
                next_arr[pos++] = 0;
                next_arr[pos++] = 1;
                next_arr[pos++] = 0;
            }
        }

        free(cur);
        cur = next_arr;
        cur_len = next_len;
        ++iter;
    }
}
