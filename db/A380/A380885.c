/*
 * A380885  a(n) is the smallest multiple m*n (m > 1) of n which contains
 *          every decimal digit of n, including repetitions.
 * https://oeis.org/A380885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A380885:
   a(n) = smallest multiple m*n (m > 1, m <= 10) of n that contains every digit of n,
   including repetitions.  */

#include <stdint.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    unsigned char need[10] = {0};
    digit_counts(sn, need);

    for (int m = 2; m <= 10; ++m) {
        seq_t p = (seq_t)m * sn;
        unsigned char have[10];
        digit_counts(p, have);

        int ok = 1;
        for (int d = 0; d < 10; ++d) {
            if (have[d] < need[d]) {
                ok = 0;
                break;
            }
        }
        if (ok)
            return p;
    }
    /* Theorem guarantees a solution for m <= 10; this line should never be reached. */
    return (seq_t)0;
}
