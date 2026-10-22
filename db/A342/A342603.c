/*
 * A342603  a(0) = 0, a(1) = 1; a(2*n) = a(n), a(2*n+1) = 6*a(n) + a(n+1).
 * https://oeis.org/A342603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2*n) = a(n); a(2*n+1) = 6*a(n) + a(n+1)
 * Range:     u8 n <= 14, u16 n <= 190, u32 n <= 9967, u64 n <= 9967,
 *            u128 n <= 9967, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* base case a(0) */
        return (seq_t)0;

    /* Start from a(0), a(1) pair */
    seq_t p = 0;          /* a(m)   for current m */
    seq_t q = 1;          /* a(m+1) */
    /* Find most significant bit of n */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >> 1) {    /* skip the highest set bit that will be processed first */
        tmp >>= 1;
        msb++;
    }

    /* Process bits from most significant down to the least significant
       (the msb itself is processed in the loop). */
    for (idx_t bit = msb; bit != (idx_t)-1; --bit) {
        int b = (n >> bit) & 1;   /* current bit value (0 or 1) */

        if (b == 0) {             /* even step: m -> 2*m */
            seq_t new_p = p;                     /* a(2*m) = a(m)   */
            seq_t new_q = 6 * p + q;             /* a(2*m+1) = 6*a(m)+a(m+1) */
            p = new_p;
            q = new_q;
        } else {                  /* odd step: m -> 2*m+1 */
            seq_t new_p = 6 * p + q;             /* a(2*m+1) = 6*a(m)+a(m+1) */
            seq_t new_q = q;                     /* a(2*m+2) = a(m+1) */
            p = new_p;
            q = new_q;
        }
    }
    return p;   /* p holds a(n) after the loop */
}
