/*
 * A287107  Positions of 2 in A287104.
 * https://oeis.org/A287107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + Sum_{k=1..n-1} d(k), with d(k)=5 if A287072(k)=0,
 *            d(k)=4 if A287072(k)=1, d(k)=3 if A287072(k)=2. This
 *            implementation directly generates the morphic word given by σ:
 *            0→10, 1→12, 2→0, starting from the seed letter '1', and locates
 *            the nth occurrence of the symbol '2'. The word is grown...
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* sequence starts at n=1 */
        return (seq_t)0;

    /* Rough upper bound on the required word length: 5·n symbols suffice for the first n occurrences of '2'. */
    size_t target_len = (size_t)(5 * n);

    /* Start with the seed letter '1'. */
    size_t cur_len = 1;
    char *cur = (char*)malloc(cur_len + 1);
    if (!cur) return (seq_t)0;
    cur[0] = '1';
    cur[1] = '\0';

    /* Expand until the desired length is reached. */
    while (cur_len < target_len) {
        size_t new_len = cur_len * 2;   /* worst‑case doubling */
        char *next = (char*)malloc(new_len + 1);
        if (!next) {
            free(cur);
            return (seq_t)0;
        }

        size_t pos = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            char c = cur[i];
            if (c == '0') {                /* 0 → 10 */
                next[pos++] = '1';
                next[pos++] = '0';
            } else if (c == '1') {          /* 1 → 12 */
                next[pos++] = '1';
                next[pos++] = '2';
            } else {                        /* 2 → 0 */
                next[pos++] = '0';
            }
        }
        next[pos] = '\0';
        cur_len = pos;

        free(cur);
        cur = next;
    }

    /* Scan the generated word to find the nth '2'. */
    size_t count = 0;
    for (size_t i = 0; i < cur_len; ++i) {
        if (cur[i] == '2') {
            ++count;
            if (count == (size_t)n) {
                free(cur);
                return (seq_t)(i + 1);    /* 1‑based positions */
            }
        }
    }

    free(cur);   /* Should not happen for valid n */
    return (seq_t)0;
}
