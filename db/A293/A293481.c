/*
 * A293481  Numbers with last digit greater than or equal to 5 (in base 10).
 * https://oeis.org/A293481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5 + 10*floor((n-1)/5) + ((n-1) mod 5)
 * Range:     u8 n <= 126, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently a(n) = 2*n + 3 - ((n-1) mod 5) */
#include <stdint.h>

seq_t a(idx_t n) {
    /* n >= 1 is assumed. */
    idx_t m = n - 1;        /* use idx_t for intermediate to avoid overflow on subtraction */
    seq_t q = (seq_t)(m / 5);          /* floor((n-1)/5) */
    seq_t r = (seq_t)(m % 5);          /* (n-1) mod 5 */
    return 5 + 10 * q + r;
}
