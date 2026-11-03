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
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search of successive integers and testing the run‑length property */

#include <stdbool.h>

static bool has_valid_run_lengths(idx_t v) {
    /* each bit of seen[i] indicates whether a run of length i has already appeared */
    bool seen[65] = {false};  /* maximum run length for a 64‑bit integer is 64 */
    while (v) {
        /* skip zeros */
        while ((v & 1) == 0) v >>= 1;
        /* count ones in the current run */
        int len = 0;
        while ((v & 1) == 1) {
            len++;
            v >>= 1;
        }
        if (seen[len])          /* repeated run length → invalid */
            return false;
        seen[len] = true;
    }
    return true;
}

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
