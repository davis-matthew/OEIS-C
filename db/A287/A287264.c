/*
 * A287264  Positions of 0 in A287263.
 * https://oeis.org/A287264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth position of the character 0 in the fixed * point of
 *            the morphism sigma: 0->0202, 1->110, 2->11
 * Range:     u8 n <= 92, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment */
seq_t a(idx_t n)
{
    /* ensure n is positive; behaviour for n==0 is undefined */
    if (n == 0)
        return (seq_t)0; /* guard, though the sequence starts at 1 */

    /* initial fixed‑point word: just the seed "0" */
    size_t len_cur = 1;
    char *cur = (char *)malloc(len_cur);
    cur[0] = '0';

    /* zero counter and array of positions (not stored because we
     * return immediately when we hit the nth zero) */
    idx_t zero_cnt = 0;
    seq_t nth_pos = 0;

    /* process the first word itself (position 1) */
    for (size_t i = 0; i < len_cur; ++i) {
        if (cur[i] == '0') {
            ++zero_cnt;
            if (zero_cnt == n) {
                nth_pos = (seq_t)(i + 1);  /* 1‑based index */
                free(cur);
                return nth_pos;
            }
        }
    }
    /* number of characters that have already been examined for zeros */
    size_t processed_len = len_cur;

    /* iterate applying the morphism until we have enough zeros */
    while (zero_cnt < n) {
        /* determine length of the next word */
        size_t next_len = 0;
        for (size_t i = 0; i < len_cur; ++i) {
            switch (cur[i]) {
                case '0': next_len += 4; break;  /* 0->0202 */
                case '1': next_len += 3; break;  /* 1->110  */
                case '2': next_len += 2; break;  /* 2->11   */
            }
        }

        /* allocate and build the next word */
        char *next = (char *)malloc(next_len);
        size_t pos = 0;
        for (size_t i = 0; i < len_cur; ++i) {
            switch (cur[i]) {
                case '0':
                    next[pos++] = '0';
                    next[pos++] = '2';
                    next[pos++] = '0';
                    next[pos++] = '2';
                    break;
                case '1':
                    next[pos++] = '1';
                    next[pos++] = '1';
                    next[pos++] = '0';
                    break;
                case '2':
                    next[pos++] = '1';
                    next[pos++] = '1';
                    break;
            }
        }

        /* free the old word and become the new one */
        free(cur);
        cur = next;
        len_cur = next_len;

        /* scan only the newly added part of the word */
        for (size_t i = processed_len; i < len_cur; ++i) {
            if (cur[i] == '0') {
                ++zero_cnt;
                if (zero_cnt == n) {
                    nth_pos = (seq_t)(i + 1);  /* 1‑based index */
                    free(cur);
                    return nth_pos;
                }
            }
        }
        processed_len = len_cur;
    }

    /* In theory we never reach this point */
    free(cur);
    return (seq_t)0;
}
