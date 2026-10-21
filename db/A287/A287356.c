/*
 * A287356  Start with 0 and repeatedly substitute 0->11, 1->12, 2->0.
 * https://oeis.org/A287356
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

/* Implements morphic sequence 0→11,1→12,2→0 starting with 1 */
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety, not required for offset 1 */

    /* initialise with the first symbol of the fixed point */
    if (seq_cache == NULL) {
        seq_cache = (seq_t *)malloc(sizeof(seq_t));
        seq_cache[0] = 1;
        seq_len = 1;
    }

    /* expand until we have at least n symbols */
    while (seq_len < n) {
        idx_t new_len = 0;
        for (idx_t i = 0; i < seq_len; ++i) {
            seq_t v = seq_cache[i];
            if (v == 0 || v == 1) new_len += 2;
            else /* v == 2 */ new_len += 1;
        }

        seq_t *new_seq = (seq_t *)malloc(new_len * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < seq_len; ++i) {
            seq_t v = seq_cache[i];
            if (v == 0) {          /* 0 → 1 1 */
                new_seq[pos++] = 1;
                new_seq[pos++] = 1;
            } else if (v == 1) {   /* 1 → 1 2 */
                new_seq[pos++] = 1;
                new_seq[pos++] = 2;
            } else {                /* 2 → 0 */
                new_seq[pos++] = 0;
            }
        }

        free(seq_cache);
        seq_cache = new_seq;
        seq_len = new_len;
    }

    return seq_cache[n - 1];
}
