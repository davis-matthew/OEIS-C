/*
 * A253490  Number of (n+1) X (3+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1655 + 247*2^n + 49*3^(n-1) for n > 4 (with exact values
 *            for n = 1,2,3,4 supplied directly)
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n <= 4 we use the known terms:
   a(1) = 1388, a(2) = 3090, a(3) = 4196, a(4) = 6931. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)1388;
    if (n == 2) return (seq_t)3090;
    if (n == 3) return (seq_t)4196;
    if (n == 4) return (seq_t)6931;
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but safe fallback */

    /* Compute 2^n and 3^(n-1) by repeated multiplication. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 = pow2 * (seq_t)2;
    }

    seq_t pow3 = 1;
    for (idx_t i = 0; i + 1 < n; ++i) {   /* i from 0 to n-2 */
        pow3 = pow3 * (seq_t)3;
    }

    /* Combine terms according to the formula. */
    seq_t term2 = ((seq_t)247) * pow2;
    seq_t term3 = ((seq_t)49) * pow3;

    return (seq_t)1655 + term2 + term3;
}
