/*
 * A382789  The number of prime factors of Euler phi of the n-th primorial
 *          number, counted with multiplicity.
 * https://oeis.org/A382789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6708, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=1..n} bigomega(p_k - 1)
 * where p_k is the k-th prime and bigomega(m) counts the prime factors of m
 * with multiplicity.
 * This implements the OEIS sequence A382789.
 */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i);          /* 1‑based index: get_prime(1)=2 */
        seq_t m = (seq_t)(p - 1);          /* p-1 is the argument to bigomega */
        sum += count_prime_factors(m);
    }
    return sum;
}
