/*
 * A247640  Number of ON cells after n generations of "Odd-Rule" cellular
 *          automaton on hexagonal lattice based on 6-celled neighborhood.
 * https://oeis.org/A247640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^{runs_of_1s(n)} * 4^{popcount(n)-runs_of_1s(n)} where
 *            runs_of_1s(n) is the number of maximal contiguous groups of 1s
 *            in the binary expansion of n, and popcount(n) is the number of
 *            1-bits. Equivalently, a(n) = 2^{2*popcount(n)-runs_of_1s(n)} *
 *            3^{runs_of_1s(n)}
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


seq_t a(idx_t n) {
    /* Count total 1 bits (t) and number of runs of 1s (r). */
    idx_t t = 0, r = 0;
    bool in_run = false;
    idx_t x = n;
    while (x) {
        bool bit = (x & 1);
        if (bit) {
            t++;
            if (!in_run) {
                r++;
                in_run = true;
            }
        } else {
            in_run = false;
        }
        x >>= 1;
    }

    /* Compute 6^r * 4^{t-r}. */
    seq_t result = 1;
    for (idx_t i = 0; i < r; ++i) {
        result *= (seq_t)6;
    }
    idx_t e4 = t - r;
    for (idx_t i = 0; i < e4; ++i) {
        result *= (seq_t)4;
    }
    return result;
}
