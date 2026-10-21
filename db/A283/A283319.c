/*
 * A283319  Image of 0 under repeated applications of the morphism 0 ->
 *          0,0,1,2, 1 -> 1,1,2, 2 -> 0,2,2.
 * https://oeis.org/A283319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A283319 – image of 0 under repeated applications of the morphism
 *
 *  Morphism (applied to the word, not to n):
 *      0 → 0,0,1,2
 *      1 → 1,1,2
 *      2 → 0,2,2
 *
 *  a(n) is the n‑th symbol of the infinite word produced by iterating
 *  this morphism starting from the single character 0 (index 1 is the
 *  first symbol of the fixed point).
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for safety return 0 if n == 0. */
    if (n == 0)
        return 0;

    /* Avoid overflow when converting to a native size_t (needed for
       array indices).  The tests are far below SIZE_MAX, so this
       branch is rarely taken. */
    if (n > (idx_t)SIZE_MAX)
        return 0;

    const size_t N = (size_t)n;            /* 1‑based target index in array form */
    unsigned char *cur = (unsigned char*)malloc(1);
    if (!cur) return 0;
    cur[0] = 0;                            /* start word "0" */
    size_t cur_len = 1;

    while (cur_len < N) {
        /* Determine capacity for the next expansion.
           We never need more than N symbols, but keep a small safety
           margin (up to 4 characters per symbol). */
        size_t cap = cur_len * 4;
        if (cap < N)  cap = N;             /* at least N elements */

        unsigned char *next = (unsigned char*)malloc(cap);
        if (!next) {
            free(cur);
            return 0;
        }

        size_t next_len = 0;
        for (size_t i = 0; i < cur_len && next_len < N; ++i) {
            unsigned char s = cur[i];
            if (s == 0) {
                next[next_len++] = 0;
                next[next_len++] = 0;
                next[next_len++] = 1;
                next[next_len++] = 2;
            } else if (s == 1) {
                next[next_len++] = 1;
                next[next_len++] = 1;
                next[next_len++] = 2;
            } else { /* s == 2 */
                next[next_len++] = 0;
                next[next_len++] = 2;
                next[next_len++] = 2;
            }
        }

        free(cur);
        cur      = next;
        cur_len  = next_len;
    }

    unsigned char result = cur[N - 1];
    free(cur);
    return (seq_t)result;   /* conversion from unsigned char to seq_t */
}
