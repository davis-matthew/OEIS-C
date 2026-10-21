/*
 * A282062  Expansion of (x + Sum_{p prime, k>=1} x^(p^k))^2.
 * https://oeis.org/A282062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   Number of representations of n as a sum of two prime powers (1
 *            included)
 * Range:     u8 n <= 2729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */

/* Return the number of ordered pairs (a,b) such that
 * a + b = n and a,b are in {1} ∪ {p^k | p prime, k ≥ 1}. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* a(0) = 0 */

    /* Allocate memory for flags up to n.  n + 1 fits in size_t. */
    size_t sz = (size_t)n + 1U;
    bool *isPrime = (bool *)calloc(sz, sizeof(bool));
    bool *primePower = (bool *)calloc(sz, sizeof(bool));
    if (!isPrime || !primePower) {
        /* In case of allocation failure, simply return 0. */
        free(isPrime);
        free(primePower);
        return 0;
    }

    /* Initialise primality flags. */
    if (sz > 0) isPrime[0] = false;
    if (sz > 1) isPrime[1] = false;
    for (size_t i = 2; i < sz; ++i)
        isPrime[i] = true;

    /* Sieve of Eratosthenes. */
    for (size_t i = 2; i * i < sz; ++i) {
        if (isPrime[i]) {
            for (size_t j = i * i; j < sz; j += i)
                isPrime[j] = false;
        }
    }

    /* Mark 1 as a prime power (special). */
    primePower[1] = true;

    /* Mark all prime powers ≤ n. */
    for (size_t p = 2; p < sz; ++p) {
        if (!isPrime[p]) continue;
        uint64_t val = p;
        /* Mark divisor p^1 */
        if (p < sz) primePower[p] = true;
        /* Mark higher powers, protecting against overflow. */
        while (val <= (uint64_t)n / p) {
            val = val * p;
            primePower[(size_t)val] = true;
        }
    }

    /* Count ordered pairs. */
    seq_t count = 0;
    for (size_t a = 1; a <= n; ++a) {
        if (!primePower[a]) continue;
        size_t b = (size_t)n - a;
        if (primePower[b]) {
            count++;
        }
    }

    free(isPrime);
    free(primePower);
    return (seq_t)count;
}
