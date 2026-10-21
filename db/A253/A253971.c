/*
 * A253971  Prime(n) is included iff prime(n) + n^2 is also prime.
 * https://oeis.org/A253971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 32, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n-th prime p_k such that p_k + k² is prime. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; below not defined */
    idx_t count  = 0;              /* how many qualifying primes found */
    idx_t index  = 0;              /* current prime's index (k)   */
    idx_t cand   = 2;              /* candidate integer to test   */

    for (;; ++cand) {
        /* test whether cand is prime */
        if (is_prime_small((seq_t)cand)) {
            ++index; /* cand is the (index)-th prime */

            /* check whether cand + index² is also prime */
            seq_t sum = (seq_t)cand + ((seq_t)index) * ((seq_t)index);
            if (is_prime_big(sum)) {
                ++count;
                if (count == n)
                    return (seq_t)cand;   /* found the requested term */
            }
        }
    }
}
