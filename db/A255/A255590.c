/*
 * A255590  Convert n to base 5, move the least significant digit to the most
 *          significant one and convert back to base 10.
 * https://oeis.org/A255590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n % 5) * 5^k + n / 5, where k = floor(log_5(n))
 * Range:     u8 n <= 126, u16 n <= 9951, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  For n = 0 the result is 0.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case directly. */
    if (n == 0)
        return 0;

    /* Find the number of base‑5 digits of n.  */
    idx_t temp = n;
    idx_t len = 0;
    while (temp != 0) {
        len++;
        temp /= 5;
    }

    /* The least significant base‑5 digit.  */
    idx_t last = n % 5U;

    /* Compute 5^(len-1) as a seq_t.  */
    seq_t pow = 1;
    for (idx_t i = 1; i < len; ++i)
        pow *= 5U;          /* seq_t multiplication by a small int */

    /* Final result: move the last digit to the front.  */
    return (seq_t)last * pow + (seq_t)(n / 5U);
}
