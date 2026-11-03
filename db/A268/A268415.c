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
 * Validated: standard, pass, 78 terms through n = 7978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced odious numbers
   a(n) = the n-th positive integer whose binary representation
   contains an odd number of runs of 1’s.
   (Runs are maximal consecutive sequences of 1-bits.) */

#include <stdbool.h>

/* Count whether a number has an odd number of runs of 1's in its binary expansion. */
static bool odd_runs_one(seq_t x)
{
    bool prev_bit = false;
    bool parity = false;          /* parity of runs seen so far */
    seq_t zero = (seq_t)0;
    seq_t one  = (seq_t)1;

    while (x != zero) {
        bool bit = (x & one) != zero;
        if (bit && !prev_bit)
            parity = !parity;      /* start of a new run of 1's */
        prev_bit = bit;
        x = x >> 1;
    }
    return parity;                 /* true iff odd number of runs */
}

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
