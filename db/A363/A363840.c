/*
 * A363840  a(n) is the number of isomorphism classes of genus 2 hyperelliptic
 *          curves over the finite field of order prime(n).
 * https://oeis.org/A363840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n = 1, a(1) = 20; for n > 1, a(n) = 2*p^3 + p^2 + 2*p - 2 +
 *            2*[p ≡ 1 (mod 3)] + 8*[p ≡ 1 (mod 5)] + 2*[p = 5] + 2*[p ≡ 1 or
 *            3 (mod 8)] where p = prime(n) (nth prime)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the helper function get_prime to retrieve the nth prime. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but safe */
    if (n == 1) return (seq_t)20;  /* special case */

    /* Retrieve nth prime (uint32_t returns 0 if n is out of range). */
    uint32_t prime_u = get_prime(n);
    if (prime_u == 0) return 0;    /* safety check */

    seq_t p = (seq_t)prime_u;
    seq_t val = 2 * p * p * p + p * p + 2 * p - 2;

    /* Indicator terms based on congruence conditions. */
    if (prime_u % 3 == 1) val += 2;
    if (prime_u % 5 == 1) val += 8;
    if (prime_u == 5) val += 2;
    if (prime_u % 8 == 1 || prime_u % 8 == 3) val += 2;

    return val;
}
