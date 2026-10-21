/*
 * A297770  Number of distinct runs in base-2 digits of n.
 * https://oeis.org/A297770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distinct binary runs in n: a(n) = #{ distinct run strings in the binary representation of n } */
/* Runs are maximal contiguous blocks of identical bits; a run is identified by its bit value and its length. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1, so this case is never used */

    uint64_t m = n;
    int prev_bit = -1;      /* -1 marks that no bit has been seen yet */
    int run_len  = 0;

    uint64_t mask0 = 0;     /* bits set for lengths of 0-runs seen */
    uint64_t mask1 = 0;     /* bits set for lengths of 1-runs seen */

    while (m != 0) {
        int bit = m & 1;
        if (bit == prev_bit) {
            ++run_len;
        } else {
            if (prev_bit != -1) {
                if (prev_bit == 0)
                    mask0 |= 1ULL << (run_len - 1);
                else
                    mask1 |= 1ULL << (run_len - 1);
            }
            prev_bit = bit;
            run_len  = 1;
        }
        m >>= 1;
    }

    /* store the final run */
    if (prev_bit != -1) {
        if (prev_bit == 0)
            mask0 |= 1ULL << (run_len - 1);
        else
            mask1 |= 1ULL << (run_len - 1);
    }

    /* popcount helper using only allowed operators */
    int pop = 0;
    uint64_t x = mask0;
    while (x) { x &= x - 1; ++pop; }
    x = mask1;
    while (x) { x &= x - 1; ++pop; }

    return (seq_t)pop;
}
