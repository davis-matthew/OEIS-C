/*
 * A268415  Balanced odious numbers: numbers with an odd number of runs of 1's
 *          in their binary expansion.
 * https://oeis.org/A268415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 8129, u32 n <= 8129, u64 n <= 8129,
 *            u128 n <= 8129, big any n
 * Helpers:   odd_runs_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced odious numbers
   a(n) = the n-th positive integer whose binary representation
   contains an odd number of runs of 1’s.
   (Runs are maximal consecutive sequences of 1-bits.) */

#include <stdbool.h>
#include "../utils/odd_runs_one.h"

/* Return the n-th balanced odious number (n >= 1). */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* start from 1 */
    idx_t count = (idx_t)0;

    while (1) {
        if (odd_runs_one(val)) {
            count++;
            if (count == n)
                return val;
        }
        val = val + (seq_t)1;
    }
}
