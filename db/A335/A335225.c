/*
 * A335225  Partitions of a stronger form of Goldbach Conjecture: the number
 *          of decompositions of an even number 2n into the sum of two odd
 *          prime numbers p and q with 0 <= q-p < n.
 * https://oeis.org/A335225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of unordered pairs of odd primes (p,q) with
 * p <= q, p+q = 2n, and q - p < n.  The function employs a simple
 * sieve of Eratosthenes up to 2n for each call. */
#include <stdlib.h>
#include <stdbool.h>

/* The types idx_t and seq_t are defined by the build system */
 
/* Return the nth term of OEIS A335225. */
seq_t a(idx_t n)
{
    /* For n < 3 no odd‑prime Goldbach partition exists. */
    if (n <= 2)
        return (seq_t)0;

    /* Build sieve up to 2n. */
    idx_t limit = 2 * n;
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime)
        return (seq_t)0;       /* Out of memory – return 0 as a safe value */

    /* Mark all numbers initially as prime (except 0,1). */
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = false;
    is_prime[1] = false;

    /* Classic Eratosthenes. */
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count valid prime pairs.  p must satisfy n/2 < p <= n. */
    seq_t count = 0;
    idx_t start = n / 2 + 1;          /* smallest integer > n/2 */
    for (idx_t p = start; p <= n; ++p) {
        if (!is_prime[p] || (p & 1u) == 0)   /* only odd primes */
            continue;
        idx_t q = 2 * n - p;                /* q is automatically odd */
        if (q >= p && q <= limit && is_prime[q]) {
            ++count;
        }
    }

    free(is_prime);
    return count;
}
