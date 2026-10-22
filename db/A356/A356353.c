/*
 * A356353  Numbers k such that A356352(k) <> 1.
 * https://oeis.org/A356353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non‑negative integer (in ascending order) whose
 *            binary expansion has all runs of equal bits of length having a
 *            gcd > 1. In particular a(1) = 0, a(2) = 3, a(3) = 7, a(4) = 12,
 *            …
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd_run_lengths, gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_run_lengths.h"
#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    if (n == 1) return 0;   /* offset 1: a(1) = 0 */

    idx_t count = 1;    /* we have already counted the element 0 */
    seq_t k = 3;        /* numbers 1 and 2 never satisfy the condition */

    for (;;) {
        seq_t g = gcd_run_lengths(k);
        if (g > 1) {
            count++;
            if (count == n) return k;
        }
        k++;
    }
}
