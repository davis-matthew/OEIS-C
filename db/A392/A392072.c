/*
 * A392072  Smallest integer k such that 1 + x^2 + y^2 = k*prime(n) has at
 *          least one solution for some integers x, y.
 * https://oeis.org/A392072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that 1 + x^2 + y^2 = k * prime(n) has a
 *            solution. This reduces to finding the smallest k (1 ≤ k <
 *            prime(n)) for which prime(n) * k – 1 is expressible as a sum of
 *            two squares
 * Range:     u8 n <= 23, u16 n <= 6838, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility to obtain the n-th prime. */
#include "../utils/get_prime.h"

/* ------------------------------------------------------------------------- */
/* Small prime list for factorisation (primes up to 120000).                  */
static uint32_t *small_primes = NULL;
static size_t small_primes_count = 0;

static void init_small_primes(void)
{
    if (small_primes != NULL) return;          /* already initialized */

    const size_t LIMIT = 120000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;                     /* insufficient memory */

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes to allocate array */
    small_primes_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++small_primes_count;

    small_primes = (uint32_t *)malloc(small_primes_count * sizeof(uint32_t));
    if (!small_primes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) small_primes[idx++] = (uint32_t)i;

    free(is_prime);
}

/* ------------------------------------------------------------------------- */
/* Determines whether a given 64‑bit integer t is a sum of two squares.      */
static bool is_sum_of_two_squares(uint64_t t)
{
    uint64_t val = t;
    for (size_t i = 0; i < small_primes_count; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * p > val) break;
        if (val % p == 0) {
            unsigned int exp = 0;
            while (val % p == 0) {
                val /= p;
                ++exp;
            }
            if ((p % 4 == 3) && (exp & 1))
                return false;               /* odd exponent of a 3mod4 prime */
        }
    }
    if (val > 1) {                           /* remaining prime factor */
        if (val % 4 == 3) return false;      /* exponent is 1 (odd) */
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function                                                     */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* behaviour below offset is irrelevant */

    /* Initialise small prime table once */
    init_small_primes();

    /* Obtain the n-th prime */
    uint32_t p = get_prime(n);

    /* Scan k = 1 .. p-1 */
    for (idx_t k = 1; k < (idx_t)p; ++k) {
        seq_t m = (seq_t)k * (seq_t)p;
        uint64_t t = (uint64_t)m - 1;        /* t = m - 1 = x^2 + y^2 */

        if (is_sum_of_two_squares(t))
            return (seq_t)k;
    }

    /* Fallback – should never be reached for valid primes. */
    return (seq_t)(p - 1);
}
