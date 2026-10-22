/*
 * A393537  a(0) = 1, a(n) = prime(n)^n * Product_{i=n+1..2*n-1} prime(i).
 * https://oeis.org/A393537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = prime(n)^n * Product_{i=n+1..2*n-1} prime(i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    if (n >= 2) {
        for (idx_t i = n + 1; i <= 2 * n - 1; ++i) {
            result *= (seq_t)get_prime(i);
        }
    }

    seq_t p = (seq_t)get_prime(n);
    result *= big_pow(p, (seq_t)n);
    return result;
}
