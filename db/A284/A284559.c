/*
 * A284559  a(n) = LCM of run lengths in binary representation of n.
 * https://oeis.org/A284559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = LCM of run lengths in binary representation of n
 * Range:     u8 n <= 9776, u16 n <= 9776, u32 n <= 9776, u64 n <= 9776,
 *            u128 n <= 9776, big any n
 * Helpers:   floor_log2_uint64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/floor_log2_uint64.h"

static seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == 0 || b == 0)
        return 0;
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    /* Special case: 0 is defined to have a run length of 1 */
    if (n == 0)
        return (seq_t)1;

    /* Determine the position of the most significant bit */
    unsigned msb = floor_log2_uint64(n);

    seq_t lcm = (seq_t)1;
    int count = 1;
    int cur_bit = (n >> msb) & 1;

    for (int i = (int)msb - 1; i >= 0; --i) {
        int bit = (n >> i) & 1;
        if (bit == cur_bit) {
            ++count;
        } else {
            lcm = lcm_seq(lcm, (seq_t)count);
            cur_bit = bit;
            count = 1;
        }
    }

    /* Process the final run length */
    lcm = lcm_seq(lcm, (seq_t)count);

    return lcm;
}
