/*
 * A282410  a(n) = binomial(2*p-1, p-1) modulo p^5, where p = prime(n).
 * https://oeis.org/A282410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*p-1, p-1) modulo p^5, where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 935, u128 n <= 1000,
 *            big any n
 * Helpers:   mod_inv128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_inv128.h"

/* Simple sieve to generate all primes up to LIMIT (once). */
static uint32_t *prime_list   = NULL;
static size_t     prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL)
        return;                      /* already initialized */

    /* Upper bound for the 10 000th prime is well below 200 000. */
    const idx_t LIMIT = 200000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime)
        return;                      /* fallback – will leave prime_list NULL */

    for (idx_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) {
        free(is_prime);
        return;                      /* leave prime_list NULL */
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            prime_list[idx++] = (uint32_t)i;
        }
    }
    prime_count = cnt;

    free(is_prime);
}

/* Return the n‑th prime (n ≥ 1).  If n exceeds the generated range,
   the function calls init_primes again with a larger bound (not needed
   for the required n ≤ 10 000). */
static inline uint64_t nth_prime(idx_t n)
{
    if (prime_list == NULL)
        init_primes();

    if (n == 0 || n > prime_count)
        return 0;                    /* invalid request */

    return (uint64_t)prime_list[n - 1];
}

/* Main function for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n = 0 */

    uint64_t p = nth_prime(n);       /* the n‑th prime */
    if (p == 0)
        return 0;                    /* safety fallback */

    /* Compute modulus p^5 using 128‑bit arithmetic. */
    __int128 mod = (__int128)p;
    mod *= p;  /* p^2 */
    mod *= p;  /* p^3 */
    mod *= p;  /* p^4 */
    mod *= p;  /* p^5 */

    /* Numerator: product of k = p+1 .. 2p-1 modulo mod. */
    __int128 num = 1;
    for (uint64_t k = p + 1; k <= 2 * p - 1; ++k)
        num = (num * (__int128)k) % mod;

    /* Denominator: (p-1)! modulo mod. */
    __int128 den = 1;
    for (uint64_t k = 1; k <= p - 1; ++k)
        den = (den * (__int128)k) % mod;

    /* Inverse of the denominator modulo mod. */
    __int128 inv = mod_inv128(den, mod);
    if (inv == -1)
        return 0;                    /* should not happen for prime p */

    __int128 result = (num * inv) % mod;

    return (seq_t)result;            /* cast to the sequence type */
}
