/*
 * A295425  a(n) = smallest number > a(n-1) such that the number of preceding
 *          terms in the sequence dividing a(n) is divisible by 4; a(1) = 2.
 * https://oeis.org/A295425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A295425
   Definition: a(1) = 2; for n>1,
   a(n) = smallest x > a(n‑1) such that count{ j<n : a(j) divides x } is divisible by 4. */

#include <stddef.h>

#define SEQ_CAPACITY 20000  /* sufficient for n up to about 10 000 */

static seq_t seq_storage[SEQ_CAPACITY];
static size_t seq_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* guard; undefined by definition */
    if (seq_len == 0) {                  /* initialise first term */
        seq_storage[0] = (seq_t)2;
        seq_len = 1;
    }
    while (seq_len < n) {
        seq_t cand = seq_storage[seq_len-1] + (seq_t)1;
        for (;;) {
            unsigned count = 0;
            for (size_t i = 0; i < seq_len; ++i) {
                if ((cand % seq_storage[i]) == (seq_t)0) ++count;
            }
            if ((count & 3) == 0) break;  /* count ≡ 0 (mod 4) */
            ++cand;
        }
        seq_storage[seq_len] = cand;
        ++seq_len;
    }
    return seq_storage[n-1];
}
