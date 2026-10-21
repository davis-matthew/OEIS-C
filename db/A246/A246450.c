/*
 * A246450  Nonnegative integers k satisfying sin(k)*cos(k+1) < 0.
 * https://oeis.org/A246450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 210, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determines the n-th non‑negative integer k such that sin(k)*cos(k+1) < 0.
   The sequence is defined by the inequality sin(k)*cos(k+1) < 0.
   Offsetting starts at n = 1: a(1) = 1. */
#include <math.h>

seq_t a(idx_t n)
{
    idx_t k = 0;          /* current candidate k */
    idx_t cnt = 0;        /* how many satisfying k's we have seen */

    while (cnt < n) {
        double s = sin((double)k);
        double c = cos((double)(k + 1));
        if (s * c < 0.0) {
            cnt++;
            if (cnt == n)
                return (seq_t)k;
        }
        k++;
    }
    /* Should never be reached for a proper n */
    return 0;
}
