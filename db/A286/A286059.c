/*
 * A286059  Fixed point of the mapping 00->001, 1->011, starting with 00.
 * https://oeis.org/A286059
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

/* Computes A286059: a(n) is the nth symbol of the fixed point of the
   rewriting system 00 → 001, 1 → 011 (single 0 remains 0).  The
   value is obtained by iteratively applying the rule until the
   current string reaches length ≥ n. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* protection for n < 1 */

    /* start with the initial word "00" */
    size_t target = (size_t)n;                  /* n ≤ 10 000 for required speed */
    size_t cur_size = 2;
    uint8_t *cur = (uint8_t *)malloc(cur_size);
    if (!cur) return (seq_t)0;                  /* allocation failure */
    cur[0] = 0; cur[1] = 0;

    while (cur_size < target) {
        size_t new_cap = cur_size * 3 + 5;       /* reserve space for expansion */
        uint8_t *next = (uint8_t *)malloc(new_cap);
        if (!next) {
            free(cur);
            return (seq_t)0;
        }
        size_t next_size = 0;
        for (size_t i = 0; i < cur_size; ) {
            if (cur[i] == 0 && i + 1 < cur_size && cur[i + 1] == 0) {
                /* 00 -> 001 */
                next[next_size++] = 0;
                next[next_size++] = 0;
                next[next_size++] = 1;
                i += 2;
            } else if (cur[i] == 1) {
                /* 1 -> 011 */
                next[next_size++] = 0;
                next[next_size++] = 1;
                next[next_size++] = 1;
                i += 1;
            } else { /* single 0 stays 0 */
                next[next_size++] = 0;
                i += 1;
            }
        }
        free(cur);
        cur = next;
        cur_size = next_size;
    }

    seq_t result = (seq_t)cur[target - 1];
    free(cur);
    return result;
}
