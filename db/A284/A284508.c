/*
 * A284508  0-limiting word of the morphism 0->1, 1->0001.
 * https://oeis.org/A284508
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

#include <stdlib.h>
#include <stdint.h>

/* Computes the nth term of A284508: the nth character of the even‑iteration
 * limit word obtained by repeatedly applying the morphism 0→1, 1→0001.
 * The function iterates until it has reached an even number of iterations
 * whose word contains the requested position, then returns that character
 * (0 or 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety; per spec n>=1 */

    size_t N = (size_t)n;
    uint8_t *cur  = (uint8_t *)malloc(N * sizeof(uint8_t));
    uint8_t *next = (uint8_t *)malloc(N * sizeof(uint8_t));
    if (!cur || !next) {
        free(cur);
        free(next);
        return 0;
    }

    cur[0] = 0;                 /* iteration 0 (even) */
    size_t cur_len = 1;
    idx_t iter = 0;             /* current iteration number */

    while (cur_len < N || (iter & 1) == 1) {   /* keep going if length short
                                                   or we are on an odd iteration */
        size_t new_len = 0;
        for (size_t i = 0; i < cur_len && new_len < N; ++i) {
            if (cur[i] == 0) {
                next[new_len++] = 1;
            } else { /* cur[i] == 1 */
                next[new_len++] = 0;
                if (new_len < N) next[new_len++] = 0;
                if (new_len < N) next[new_len++] = 0;
                if (new_len < N) next[new_len++] = 1;
            }
        }
        ++iter;
        cur_len = new_len;
        /* swap the two buffers */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = (seq_t)cur[N - 1];
    free(cur);
    free(next);
    return result;
}
