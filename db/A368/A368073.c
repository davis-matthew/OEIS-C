/*
 * A368073  Number of representations of n as the difference of two positive
 *          square pyramidal numbers.
 * https://oeis.org/A368073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as the difference of two
 *            positive square pyramidal numbers
 * Range:     u8 n <= 80, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   pyramid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pyramid.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not defined for n = 0, but safe guard */

    seq_t target = (seq_t)n;
    seq_t count   = 0;

    for (idx_t a = 1; ; ++a) {
        seq_t a_seq = (seq_t)a;
        /* Minimum difference for this 'a' is P(a) - P(a-1) = a^2.
           If a^2 > target, no larger 'a' will give a difference <= target. */
        if (a_seq * a_seq > target) break;

        seq_t Pa = pyramid(a_seq);
        for (idx_t b = 1; b < a; ++b) {
            seq_t Pb   = pyramid((seq_t)b);
            seq_t diff = Pa - Pb;

            if (diff == target) {
                ++count;
            } else if (diff < target) {
                /* Further increasing b will only decrease the difference. */
                break;
            }
        }
    }

    return count;
}
