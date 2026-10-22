/*
 * A348841  Number of primes with even exponents >= 2 in the prime power
 *          factorization of n!, for n >= 1.
 * https://oeis.org/A348841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ p ≤ n | e_p(n!) is even and ≥ 2 }
 * Range:     u8 n <= 5865, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where e_p(n!) = Σ_{k≥1} ⌊ n / p^k ⌋ is the exponent of prime p in n! */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)                /* no primes ≤ n */
        return (seq_t)0;

    /* ---------- Sieve of Eratosthenes up to n ---------- */
    idx_t limit = n;
    uint8_t *is_comp = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!is_comp)
        return (seq_t)0;      /* allocation failed */

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* Count primes and store them */
    /* Upper bound for number of primes ≤ n is n/ln(n)+10 for n≥2 */
    size_t prime_est = (size_t)(limit / (limit > 15 ? (idx_t)(8) : (idx_t)(1)) + 10);
    uint32_t *primes = (uint32_t *)malloc(prime_est * sizeof(uint32_t));
    if (!primes) {
        free(is_comp);
        return (seq_t)0;
    }

    size_t pcnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) {
            if (pcnt >= prime_est) {          /* reallocate if needed */
                prime_est *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, prime_est * sizeof(uint32_t));
                if (!tmp) {
                    free(is_comp);
                    free(primes);
                    return (seq_t)0;
                }
                primes = tmp;
            }
            primes[pcnt++] = (uint32_t)i;
        }

    free(is_comp);

    /* ---------- Count primes with even exponent ≥ 2 ---------- */
    seq_t result = 0;

    for (size_t idx = 0; idx < pcnt; ++idx) {
        idx_t p   = (idx_t)primes[idx];
        idx_t exp = 0;
        idx_t div = p;

        while (div <= limit) {
            exp += limit / div;
            if (div > limit / p)
                break;
            div *= p;
        }

        if ((exp & 1ULL) == 0 && exp >= 2)
            result += 1;
    }

    free(primes);
    return result;
}
