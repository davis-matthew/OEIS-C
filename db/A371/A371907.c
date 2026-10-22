/*
 * A371907  a(n) = sum of 2^(k-1) such that floor(n/prime(k)) is even.
 * https://oeis.org/A371907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ 2^(k-1) over primes p=prime(k) <= n with floor(n/p)
 *            even (prime indices k are 1‑based)
 * Range:     u8 n <= 45, u16 n <= 117, u32 n <= 273, u64 n <= 625,
 *            u128 n <= 1453, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Generate primes up to n with a simple sieve */
    idx_t limit = n;
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime)
        return 0;          /* allocation failure – return 0 as a safe default */

    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    seq_t sum = 0;
    idx_t k = 1;           /* prime index (1‑based) */
    for (idx_t p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            if (((n / p) & 1) == 0) {
                seq_t term = (seq_t)1 << (k - 1);
                sum += term;
            }
            ++k;
        }
    }

    free(is_prime);
    return sum;
}
