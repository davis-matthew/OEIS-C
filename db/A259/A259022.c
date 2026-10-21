/*
 * A259022  Period 9 sequence [ 1, -1, -1, 1, 0, -1, 1, 1, -1, ...].
 * https://oeis.org/A259022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in (1-x)(1-x^2)(1-x^5). The
 *            generating function expands to (1 - x - x^2 + x^3 - x^5 + x^6 +
 *            x^7 - x^8) so the sequence is periodic with period 9 and takes
 *            values 1,-1,-1,1,0,-1,1,1,-1
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Reduce n modulo 9 – the sequence is 9‑periodic. */
    idx_t m = n % (idx_t)9;

    /* Compute the unique subset of {1,2,5} whose sum equals m.
       The coefficient is +1 if the subset size is even, -1 otherwise.
       (No hard‑coded table is used.) */
    int coeff = 0;
    /* masks 0..7 represent the 3‑bit subsets. */
    for (int mask = 0; mask < 8; ++mask) {
        int sum = 0, bits = 0;
        if (mask & 1) { sum += 1; bits++; }
        if (mask & 2) { sum += 2; bits++; }
        if (mask & 4) { sum += 5; bits++; }
        if ((uint64_t)sum == m) {
            coeff += (bits & 1) ? -1 : 1;
        }
    }
    return (seq_t)coeff;
}
