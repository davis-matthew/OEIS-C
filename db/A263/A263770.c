/*
 * A263770  Smallest prime q such that (prime(n)^2 + q*prime(n))/(prime(n) +
 *          1) is an integer.
 * https://oeis.org/A263770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A263770:  a(n) = the smallest prime q such that
   (prime(n)^2 + q*prime(n))/(prime(n)+1) is an integer.
   Equivalently, q ≡ 1 (mod prime(n)+1).  The routine implements
   this by a brute‑force search for the smallest such prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; behaviour for n == 0 is irrelevant. */
    if (n == 0) return 0;

    /* Compute the n‑th prime p = prime(n). */
    seq_t p = (seq_t)get_prime(n);

    /* Modulus for the congruence.  We need the smallest prime
       congruent to 1 modulo m = p + 1. */
    seq_t m = p + 1;

    /* Search for the first prime q = k*m + 1.  We start with k = 1,
       because q = 1*m + 1 = m + 1 already satisfies the congruence. */
    for (idx_t k = 1; ; ++k) {
        seq_t q = (seq_t)k * m + 1;
        if (is_prime_big(q))
            return q;
    }
}
