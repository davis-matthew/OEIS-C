/*
 * A271181  Read A002942 digit-by-digit, beginning with an initial 0.
 * https://oeis.org/A271181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1203, u32 n <= 5543, u64 n <= 5543,
 *            u128 n <= 5543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A271181: a(n) is the digit at index n of the infinite
   concatenation of the reversed decimal representations of
   n² for n = 1, 2, 3, ... ; a(0) = 0. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special‑case the first digit, which is always 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Count how many digits have been produced so far. */
    idx_t idx = 0;

    for (idx_t m = 1; ; ++m) {
        /* Compute m², using seq_t to avoid overflow. */
        seq_t sq = (seq_t)m * (seq_t)m;
        seq_t x  = sq;

        /* Emit the digits of sq in reverse order. */
        while (x) {
            seq_t d = x % (seq_t)10;   /* current least‑significant digit */
            ++idx;
            if (idx == n) {
                return d;              /* the desired digit */
            }
            x /= (seq_t)10;
        }
    }
}
