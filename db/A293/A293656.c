/*
 * A293656  a(n) = binomial(n+1,2)*n!/n!!.
 * https://oeis.org/A293656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,2) * n! / n!!
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n * ((seq_t)n + 1) / 2;   /* binomial(n+1,2) */

    if ((n & 1) == 0) {        /* n is even: multiply the odd factors */
        for (idx_t i = 1; i <= n / 2; ++i) {
            result *= (seq_t)(2 * i - 1);   /* 1,3,5,...,n-1 */
        }
    } else {                   /* n is odd: multiply the even factors */
        for (idx_t i = 1; i <= (n - 1) / 2; ++i) {
            result *= (seq_t)(2 * i);     /* 2,4,6,...,n-1 */
        }
    }
    return result;
}
