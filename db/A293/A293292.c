/*
 * A293292  Numbers with last digit less than 5 (in base 10).
 * https://oeis.org/A293292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n-1) + 5*floor((n-1)/5)
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 0, so n must be at least 1.  Return 0 for n=0 to avoid UB. */
    if (n == 0)
        return (seq_t)0;

    /* Use idx_t for intermediate calculations to avoid overflow for large n. */
    idx_t m = n - 1;                 /* m = n-1 */
    idx_t part = m / 5;              /* floor((n-1)/5) */
    idx_t res = m + 5 * part;        /* (n-1) + 5*floor((n-1)/5) */

    return (seq_t)res;
}
