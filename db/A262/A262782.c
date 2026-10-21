/*
 * A262782  a(n) = sum_{k=1..n} 3^prime(k).
 * https://oeis.org/A262782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} 3^prime(k) (prime(1)=2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset starts at 1, but guard */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        uint32_t p = get_prime(k);          /* k-th prime */
        seq_t term = big_pow((seq_t)3, (seq_t)p);
        sum += term;
    }
    return sum;
}
