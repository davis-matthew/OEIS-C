/*
 * A348794  a(n) = number of 3-regular one-face rooted maps on orientable
 *          surfaces of genus n.
 * https://oeis.org/A348794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(6n-3)!/(12^n*n!*(3n-2)!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   prime_factorial_exponent, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/prime_factorial_exponent.h"

/* ---- Prime sieve for values up to 60000 ------------------------------------ */
#define MAX_PRIME_LIMIT 60000
/* Rough upper bound on number of primes up to MAX_PRIME_LIMIT */
#define MAX_PRIME_COUNT 6000

static uint32_t prime_list[MAX_PRIME_COUNT];
static size_t prime_count = 0;
static bool primes_ready = false;

/* Generate all primes up to MAX_PRIME_LIMIT once */
static void init_primes(void)
{
    if (primes_ready)
        return;

    /* Simple Sieve of Eratosthenes */
    bool *is_prime = (bool *)calloc(MAX_PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime)
        return;   /* allocation failure – unlikely */
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_prime[j] = false;

    for (size_t i = 2; i <= MAX_PRIME_LIMIT && prime_count < MAX_PRIME_COUNT; ++i)
        if (is_prime[i])
            prime_list[prime_count++] = (uint32_t)i;

    free(is_prime);
    primes_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset 1: undefined for n < 1 */

    init_primes();

    idx_t m1 = 6 * n - 3;      /* (6n-3)!  */
    idx_t m2 = 3 * n - 2;      /* (3n-2)!  */

    seq_t result = 1;          /* multiplicative identity */

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (p > m1)
            break;

        idx_t exp = prime_factorial_exponent(m1, p)
                  - prime_factorial_exponent(n, p)
                  - prime_factorial_exponent(m2, p);

        /* subtract exponent of 12^n : 2^(2n) * 3^n */
        if (p == 2)
            exp -= 2 * n;
        if (p == 3)
            exp -= n;

        /* multiply by the leading factor 2 */
        if (p == 2)
            exp += 1;

        if (exp > 0) {
            seq_t powp = seq_pow((seq_t)p, (idx_t)exp);
            result = seq_mul(result, powp);
        }
    }

    return result;
}
