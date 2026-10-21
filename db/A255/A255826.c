/*
 * A255826  a(n) = n for n < 6; a(6n) = a(n); if every 6th term (a(6), a(12),
 *          a(18),...) is deleted, this gives back the original sequence.
 * https://oeis.org/A255826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n < 6; a(n) = a(n/6) if n mod 6 == 0; a(n) = a(n -
 *            floor(n/6)) otherwise
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This iterative implementation follows the definitions above. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Reduce n until it falls into the base case n < 6 */
    while (n > 5) {
        idx_t q = n / 6;          /* floor(n/6) */
        if (n % 6 == 0) {
            n = q;                /* divisible by 6 → a(n/6) */
        } else {
            n = n - q;            /* otherwise → a(n - floor(n/6)) */
        }
    }
    return (seq_t)n;  /* n is now between 1 and 5 inclusive */
}
