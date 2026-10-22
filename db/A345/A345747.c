/*
 * A345747  a(n) = n! * Sum_{k=0..floor(n/2)} k^(n - 2*k)/k!.
 * https://oeis.org/A345747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} k^(n - 2*k)/k!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1; /* a(0) = 1 */

    seq_t fact_n = factorial(n);   /* n! */
    seq_t coeff   = fact_n;         /* C_k = n! / k! */
    seq_t result  = 0;

    for (idx_t k = 1; k <= n / 2; ++k)
    {
        coeff = coeff / k;                         /* C_k = n! / k! */
        seq_t power = big_pow((seq_t)k, (seq_t)(n - 2 * k));
        result += coeff * power;
    }

    return result;
}
