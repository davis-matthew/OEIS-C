/*
 * A318924  In the binary Champernowne word A030190, shorten each run of
 *          consecutive identical bits by one bit.
 * https://oeis.org/A318924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9463, u16 n <= 9463, u32 n <= 9463, u64 n <= 9463,
 *            u128 n <= 9463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived by generating the binary Champernowne word A030190 and
 * outputting each bit except the first bit of every consecutive run
 * of identical bits (effectively shortening each run by one). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* We need to generate (n + 1) output bits */
    idx_t needed = n + 1;
    idx_t outCnt = 0;

    int prev_bit = 2;            /* sentinel: no previous bit yet */
    int first_in_run = 1;        /* true when current run has not emitted any bit yet */
    int cur_bit = 0;             /* holds the bit to return when done */

    for (idx_t num = 0; ; ++num)
    {
        /* Produce bits of 'num' from most significant to least */
        if (num == 0)
        {
            /* Special case: binary of 0 is single bit '0' */
            int bit = 0;
            if (bit != prev_bit) {
                /* Start new run: skip this bit */
                prev_bit = bit;
                first_in_run = 1;
            } else {
                /* Second or later bit in run: emit */
                if (first_in_run) first_in_run = 0;
                cur_bit = bit;
                ++outCnt;
                if (outCnt == needed) return (seq_t)cur_bit;
            }
            continue;
        }

        uint64_t m = (uint64_t)num;
        /* Find index of most significant bit */
        unsigned int msb = 63 - __builtin_clzll(m);
        for (int i = msb; i >= 0; --i)
        {
            int bit = (m >> i) & 1U;
            if (bit != prev_bit) {
                /* Start new run: skip this bit */
                prev_bit = bit;
                first_in_run = 1;
            } else {
                /* Subsequent bit in the same run: emit */
                if (first_in_run) first_in_run = 0;
                cur_bit = bit;
                ++outCnt;
                if (outCnt == needed) return (seq_t)cur_bit;
            }
        }
    }
}
