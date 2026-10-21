/*
 * A285431  Fixed point of the morphism 0->11, 1-> 110.
 * https://oeis.org/A285431
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

/* Compute a(n) as the fixed point of the morphism 0->11, 1->110. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, return 0 as a safe value */

    /* Static storage for the growing word. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    /* Grow the word until it contains at least n symbols. */
    if (seq_len < (size_t)n) {
        /* Initialise with the first symbol 1. */
        if (seq_len == 0) {
            seq = (seq_t *)malloc(sizeof(seq_t));
            seq[0] = (seq_t)1;
            seq_len = 1;
        }

        while (seq_len < (size_t)n) {
            /* Compute the length after applying the morphism to all current symbols. */
            size_t new_len = 0;
            for (size_t i = 0; i < seq_len; ++i)
                new_len += (seq[i] & (seq_t)1) ? 3 : 2;   /* 1→110, 0→11 */

            /* Allocate new buffer and copy the transformed word. */
            seq_t *new_seq = (seq_t *)malloc(new_len * sizeof(seq_t));
            size_t pos = 0;
            for (size_t i = 0; i < seq_len; ++i) {
                if (seq[i] & (seq_t)1) {                /* symbol 1 */
                    new_seq[pos++] = (seq_t)1;
                    new_seq[pos++] = (seq_t)1;
                    new_seq[pos++] = (seq_t)0;
                } else {                                /* symbol 0 */
                    new_seq[pos++] = (seq_t)1;
                    new_seq[pos++] = (seq_t)1;
                }
            }

            free(seq);
            seq = new_seq;
            seq_len = new_len;
        }
    }

    return seq[n - 1];
}
