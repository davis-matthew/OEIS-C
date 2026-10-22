/*
 * A328592  Numbers whose binary expansion has all different lengths of runs
 *          of 1's.
 * https://oeis.org/A328592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth number whose binary expansion has all different
 *            lengths of runs of 1’s
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   has_valid_run_lengths
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search of successive integers and testing the run‑length property */

#include <stdbool.h>
#include "../utils/has_valid_run_lengths.h"

seq_t a(idx_t n) {
    idx_t count = 0;      /* how many valid numbers have been found so far */
    idx_t candidate = 0;  /* current integer to test */

    while (1) {
        if (has_valid_run_lengths(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;  /* cast to the build’s seq_t type */
        }
        candidate++;
    }
}
