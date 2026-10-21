/*
 * A253769  Sum of number of divisors of all positive integers <= prime(n).
 * https://oeis.org/A253769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{prime(n)} d(k) (sum of divisor counts up to the
 *            n‑th prime)
 * Range:     u8 n <= 17, u16 n <= 927, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    uint64_t p = get_prime(n);            /* n-th prime */
    seq_t sum = 0;
    for (uint64_t i = 1; i <= p; ++i) {
        sum += (seq_t)divisor_count_u64(i); /* add d(i) */
    }
    return sum;
}
