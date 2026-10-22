/*
 * A369057  Total number of representations of natural numbers in range 1 ..
 *          4n-1 as sums of the form p*q + p*r + q*r, with three odd primes p
 *          <= q <= r.
 * https://oeis.org/A369057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count the representations k = p*q + p*r + q*r with odd primes p
 *            ≤ q ≤ r and 1 ≤ k ≤ 4n-1. a(n) is this count for the range [1,
 *            4n‑1]
 * Range:     u8 n <= 225, u16 n <= 9885, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    /* Upper bound for the sum p*q+p*r+q*r. */
    const idx_t M = 4 * n - 1;

    /* --- Sieve of Eratosthenes (boolean array) --- */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)(M + 1), sizeof(uint8_t));
    if (!is_prime) return (seq_t)0;
    /* Mark all numbers ≥2 as potential primes */
    for (idx_t i = (idx_t)2; i <= M; ++i) is_prime[(size_t)i] = 1;
    /* 0 and 1 are not prime */
    is_prime[0] = 0;
    if (M >= 1) is_prime[1] = 0;
    for (idx_t i = (idx_t)2; (i * i) <= M; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= M; j += i)
                is_prime[(size_t)j] = 0;
        }
    }

    /* --- Prefix count of odd primes up to each index --- */
    uint32_t *prime_prefix = (uint32_t *)malloc((size_t)(M + 1) * sizeof(uint32_t));
    if (!prime_prefix) { free(is_prime); return (seq_t)0; }
    prime_prefix[0] = 0;
    for (idx_t i = (idx_t)1; i <= M; ++i) {
        prime_prefix[(size_t)i] = prime_prefix[(size_t)(i - 1)] +
                                  ((i >= (idx_t)3 && (i & (idx_t)1) && is_prime[(size_t)i]) ? 1 : 0);
    }

    /* --- List of odd primes (vector) --- */
    size_t primes_cnt = prime_prefix[(size_t)M];
    uint64_t *primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); free(prime_prefix); return (seq_t)0; }
    size_t pidx = 0;
    for (idx_t i = (idx_t)3; i <= M; i += (idx_t)2) {
        if (is_prime[(size_t)i])
            primes[pidx++] = (uint64_t)i;
    }

    /* --- Count all admissible triples (p ≤ q ≤ r) --- */
    seq_t total = (seq_t)0;
    for (size_t ip = (size_t)0; ip < primes_cnt; ++ip) {
        uint64_t p = primes[ip];
        for (size_t iq = ip; iq < primes_cnt; ++iq) {
            uint64_t q = primes[iq];
            uint64_t pq = p * q;
            if (pq > (uint64_t)M) break;                 /* q too large for this p */
            uint64_t num = (uint64_t)M - pq;
            uint64_t denom = p + q;
            uint64_t rmax = num / denom;
            if (rmax < q) continue;                     /* no suitable r */
            uint32_t cnt = prime_prefix[(size_t)rmax] -
                           prime_prefix[(size_t)(q - 1)];
            total += (seq_t)cnt;
        }
    }

    /* --- Clean up --- */
    free(is_prime);
    free(prime_prefix);
    free(primes);

    return total;
}
