/*
 * A287086  Start with 0 and repeatedly substitute 0->01, 1->22, 2->0.
 * https://oeis.org/A287086
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

/* Recurrence: the sequence is the fixed point of the morphism
 *   0 → 01,  1 → 22,  2 → 0.
 * a(1) = 0.
 *
 * The implementation keeps a growing array of the generated terms
 * and expands it until the requested index is reached.
 */
#include <stdlib.h>

static seq_t *st_seq = NULL;
static idx_t st_len = 0;

seq_t a(idx_t n)
{
    if (n == 0)    /* positions below the offset are irrelevant */
        return 0;

    if (st_len < n) {
        if (st_len == 0) {
            st_len = 1;
            st_seq = (seq_t *)malloc(sizeof(seq_t));
            st_seq[0] = 0;
        }
        while (st_len < n) {
            idx_t new_cap = st_len * 2;
            seq_t *next = (seq_t *)malloc(sizeof(seq_t) * new_cap);
            idx_t pos = 0;
            for (idx_t i = 0; i < st_len; ++i) {
                seq_t c = st_seq[i];
                if (c == 0) {
                    next[pos++] = 0;
                    next[pos++] = 1;
                } else if (c == 1) {
                    next[pos++] = 2;
                    next[pos++] = 2;
                } else {   /* c == 2 */
                    next[pos++] = 0;
                }
            }
            idx_t new_len = pos;
            free(st_seq);
            st_seq = next;
            st_len = new_len;
        }
    }
    return st_seq[n - 1];
}
