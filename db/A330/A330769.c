/*
 * A330769  a(n) = Product_{k=max(1,n)..2*n} prime(k).
 * https://oeis.org/A330769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=max(1,n)}^{2n} prime(k) (OEIS A330769)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty product) */
    if (n == 0)
        return 1;

    /* Indices are 1‑based in OEIS definition */
    idx_t low  = n;          /* max(1,n) */
    idx_t high = 2 * n;      /* inclusive upper bound */

    /* Upper bound for the sieve value:
       For high ≤ 20000, 30*high+100 is always ≥ the high‑th prime. */
    size_t bound = 30 * high + 100;

    /* Simple Sieve of Eratosthenes */
    bool *is_prime = (bool *)malloc(bound + 1);
    if (!is_prime)
        return 0;             /* allocation failed */

    memset(is_prime, true, bound + 1);
    is_prime[0] = false;
    is_prime[1] = false;

    for (size_t i = 2; i * i <= bound; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= bound; j += i)
                is_prime[j] = false;

    /* Gather first `high` primes into an array */
    uint32_t *primes = (uint32_t *)malloc(high * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        return 0;             /* allocation failed */
    }

    size_t pc = 0;
    for (size_t i = 2; i <= bound && pc < high; ++i)
        if (is_prime[i])
            primes[pc++] = (uint32_t)i;

    /* Compute the product */
    seq_t prod = 1;
    for (idx_t idx = low - 1; idx < high; ++idx)
        prod *= (seq_t)primes[idx];

    free(primes);
    free(is_prime);
    return prod;
}
