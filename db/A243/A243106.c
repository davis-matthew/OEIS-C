/*
 * A243106  a(n) = Sum_{k=1..n} (-1)^isprime(k)*10^k.
 * https://oeis.org/A243106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Sum_{k=1..n} (-1)^isprime(k) * 10^k
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 38,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset behaviour is irrelevant for n < 1 */
        return 0;

    seq_t sum = 0;
    seq_t pow  = (seq_t)10;   /* 10^1 */

    for (idx_t k = 1; k <= n; ++k) {
        if (is_prime(k))
            sum -= pow;     /* prime -> negative sign */
        else
            sum += pow;     /* composite or 1 -> positive sign */

        pow *= (seq_t)10;   /* next power of 10 */
    }

    return sum;
}
