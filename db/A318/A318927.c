/*
 * A318927  Take the binary expansion of n, starting with the most significant
 *          bit, and concatenate the lengths of the runs.
 * https://oeis.org/A318927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal concatenation of the run lengths of the binary
 *            expansion of n, treating each run length as a decimal number and
 *            concatenating them
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 1022, u64 n <= 1022,
 *            u128 n <= 1022, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t v = n;
    idx_t msb = floor_log2_uint64(v);
    idx_t current = (v >> msb) & 1;
    idx_t run_len = 1;

    for (idx_t pos = msb; pos > 0; --pos) {
        idx_t bit = (v >> (pos - 1)) & 1;
        if (bit == current) {
            ++run_len;
        } else {
            seq_t run = (seq_t)run_len;
            /* number of decimal digits in run */
            int d = 0;
            seq_t tmp = run;
            do { ++d; tmp /= 10; } while (tmp > 0);
            /* compute 10^d */
            seq_t pow10 = 1;
            for (int i = 0; i < d; ++i) pow10 *= 10;
            result = result * pow10 + run;
            current = bit;
            run_len = 1;
        }
    }
    /* append the final run length */
    seq_t run = (seq_t)run_len;
    int d = 0;
    seq_t tmp = run;
    do { ++d; tmp /= 10; } while (tmp > 0);
    seq_t pow10 = 1;
    for (int i = 0; i < d; ++i) pow10 *= 10;
    result = result * pow10 + run;
    return result;
}
