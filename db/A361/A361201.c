/*
 * A361201  Product of the right half (exclusive) of the multiset of prime
 *          factors of n; a(1) = 0.
 * https://oeis.org/A361201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = product of the last ⌊#prime_factors(n)/2⌋ factors of n
 *            after prime factorization with multiplicity sorted in
 *            non‑decreasing order
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for n ≥ 1.  seq_t and idx_t are defined in the build. */
seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)0;

    /* Factorize n by trial division.  The result fits in 64 bits, so
       storing the factors in a small fixed array is safe. */
    uint64_t temp = (uint64_t)n;
    uint64_t factors[128];
    size_t cnt = 0;

    /* Handle factor 2. */
    while ((temp & 1ULL) == 0) {
        factors[cnt++] = 2ULL;
        temp >>= 1;
    }

    /* Handle odd factors. */
    for (uint64_t p = 3ULL; p * p <= temp; p += 2ULL) {
        while (temp % p == 0) {
            factors[cnt++] = p;
            temp /= p;
        }
    }

    /* If anything remains, it is a prime factor > sqrt(original n). */
    if (temp > 1) {
        factors[cnt++] = temp;
    }

    /* Compute the product of the right half (exclusive) of the multiset of
       prime factors.  The array 'factors' is already sorted because we
       encountered prime factors in increasing order during division. */
    size_t half  = cnt / 2;              /* floor(cnt/2) */
    size_t start = cnt - half;           /* indices to multiply */
    seq_t prod = (seq_t)1;

    for (size_t i = start; i < cnt; ++i) {
        prod = prod * (seq_t)factors[i];
    }

    return prod;
}
