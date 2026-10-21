/*
 * A278500  a(n) = largest k such that n+1 = a prime, n+2 = 2 * a prime, ...,
 *          n+k is k times a prime, a(n) = 0 if n+1 is not a prime.
 * https://oeis.org/A278500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that n+i = i * p_i for all 1 ≤ i ≤ k,
 *            where each p_i is prime
 * Range:     u8 n <= 6467, u16 n <= 9951, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computations use only integer arithmetic and the provided primality test. */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* i starts at 1 */
    idx_t i = 1;
    for (;; ++i) {
        idx_t m = n + i;       /* current number n + i */
        if (m % i != 0) break; /* must be divisible by i */
        idx_t q = m / i;       /* the candidate prime factor */
        if (!is_prime_small((uint64_t)q)) break;
    }
    /* The loop exits with i being the first index that fails,
       so the answer is i-1. */
    return (seq_t)(i - 1);
}
