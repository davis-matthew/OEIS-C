/*
 * A374176  a(n) is the maximum number of consecutive bit changes in the
 *          binary representation of n.
 * https://oeis.org/A374176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum length of a consecutive run of changes between
 *            adjacent bits in the binary representation of n (OEIS A374176)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* for safety, handle the offset below 1 gracefully */
    if (n == 0U) {
        return 0;
    }

    /* bit length of n */
    unsigned int bl = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bl;
        tmp >>= 1;
    }

    unsigned int max_run = 0;
    unsigned int curr   = 0;
    idx_t prev_bit = n & 1;          /* bit 0 */

    /* iterate over the remaining bits, counting runs of alternations */
    for (unsigned int i = 1; i < bl; ++i) {
        idx_t curr_bit = (n >> i) & 1;
        if (curr_bit != prev_bit) {
            ++curr;
            if (curr > max_run) {
                max_run = curr;
            }
        } else {
            curr = 0;
        }
        prev_bit = curr_bit;
    }

    return (seq_t)max_run;
}
