/*
 * A301506  Number of integers less than or equal to n whose largest prime
 *          factor is 5.
 * https://oeis.org/A301506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers <= n whose largest prime factor is 5
 * Range:     u8 n <= 521, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    /* Powers of 5, 2, and 3 that are needed for n <= 10000. */
    const uint64_t POW5[6] = {1, 5, 25, 125, 625, 3125};
    const uint64_t POW2[14] = {1, 2, 4, 8, 16, 32, 64, 128,
                               256, 512, 1024, 2048, 4096, 8192};
    const uint64_t POW3[9] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561};

    seq_t count = 0;

    for (idx_t c = 1; c < 6; ++c) {
        seq_t base5 = (seq_t)POW5[c];
        if (base5 > (seq_t)n)
            break;
        seq_t limit5 = (seq_t)n / base5;

        for (idx_t a = 0; a < 14; ++a) {
            seq_t base5_2 = base5 * (seq_t)POW2[a];
            if (base5_2 > (seq_t)n)
                break;
            seq_t limit5_2 = (seq_t)n / base5_2;

            for (idx_t b = 0; b < 9; ++b) {
                seq_t prod = base5_2 * (seq_t)POW3[b];
                if (prod > (seq_t)n)
                    break;
                count++;
            }
        }
    }

    return count;
}
