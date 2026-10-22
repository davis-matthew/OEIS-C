/*
 * A382185  a(n) is the n-th tribonacci number modulo the n-th prime.
 * https://oeis.org/A382185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = tribonacci(n) modulo the n‑th prime (OEIS A382185)
 * Range:     u8 n <= 61, u16 n <= 6585, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n = 0 is outside the OEIS offset; return 0 safely */
    if (n == 0) return (seq_t)0;

    /* Retrieve the n‑th prime. get_prime returns a 32‑bit value. */
    uint32_t prime = get_prime(n);
    unsigned long long mod = (unsigned long long)prime;

    /* Tribonacci initial values (indices start at 1). */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;

    /* Compute T(n) modulo prime using a rolling window; no large
       intermediate values are needed, all arithmetic fits in 64 bits. */
    unsigned long long t1 = 0, t2 = 1, t3 = 1;   /* T(1), T(2), T(3) */
    for (idx_t i = 4; i <= n; ++i) {
        unsigned long long next = (t1 + t2 + t3) % mod;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }

    return (seq_t)t3;
}
