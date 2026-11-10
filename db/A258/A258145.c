/*
 * A258145  Row lengths of the irregular array in A256598.
 * https://oeis.org/A258145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A078719(2*n+1): number of odd terms in the Collatz
 *            trajectory of 2n+1 down to 1
 * Range:     u8 n <= 12, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 2 + 1;
    seq_t odds = 1; /* x is odd (the start) */
    while (x != 1) {
        x = x * 3 + 1;
        while ((x & 1) == 0) x >>= 1;
        odds++;
    }
    return odds;
}
