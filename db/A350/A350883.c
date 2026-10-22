/*
 * A350883  Sum of the smaller parts of the partitions of n into two prime
 *          parts.
 * https://oeis.org/A350883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p prime, p ≤ n/2, n−p prime} p The function
 *            implements a straightforward sieve of Eratosthenes up to n and
 *            then sums the smaller prime part of each Goldbach partition of n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* For n < 4 the result is zero; the loop below will also
       produce zero, but we handle it explicitly for clarity. */
    if (n < 4) {
        return 0;
    }

    /* Allocate a boolean array for prime flags. */
    size_t limit = (size_t)n + 1;
    unsigned char *is_prime = (unsigned char *)malloc(limit);
    if (!is_prime) {
        /* Allocation failure – return 0 as a safe fallback. */
        return 0;
    }

    /* Initialise all entries as prime; clear 0 and 1. */
    for (size_t i = 0; i < limit; ++i) {
        is_prime[i] = 1;
    }
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* Sieve of Eratosthenes. */
    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= n; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Sum the smaller prime part of each two‑prime partition. */
    seq_t sum = 0;
    size_t half = n / 2;
    for (size_t p = 2; p <= half; ++p) {
        if (is_prime[p] && is_prime[n - p]) {
            sum += (seq_t)p;
        }
    }

    free(is_prime);
    return sum;
}
