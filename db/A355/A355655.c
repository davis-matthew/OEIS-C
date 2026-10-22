/*
 * A355655  a(n) = 1 if the smallest b > 1 such that b^(p-1) == 1 (mod p^2) is
 *          prime, 0 otherwise, with p = prime(n).
 * https://oeis.org/A355655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the smallest b > 1 such that b^(p-1) ≡ 1 (mod p²) is
 *            prime, 0 otherwise, where p = prime(n)
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   get_prime, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"     /* provides seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/is_prime.h"    /* provides bool is_prime(idx_t n); */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Obtain the n-th prime p. */
    uint32_t p = get_prime(n);

    /* Compute modulus p^2. */
    seq_t mod = (seq_t)p * (seq_t)p;

    /* Search for the smallest base b > 1 satisfying b^(p-1) ≡ 1 (mod p^2). */
    for (seq_t b = 2;; ++b)
    {
        if (mod_pow(b, (uint64_t)(p - 1), mod) == 1)
        {
            /* Return 1 if that base is prime, 0 otherwise. */
            return is_prime((idx_t)b) ? (seq_t)1 : (seq_t)0;
        }
    }
}
