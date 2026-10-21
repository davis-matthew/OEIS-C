/*
 * A244583  a(n) = sum of all divisors of all positive integers <= prime(n).
 * https://oeis.org/A244583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{p_n} sigma(k) where p_n is the n-th prime and
 *            sigma(k) is the sum of the divisors of k
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ---------- Prime generation ---------- */
static uint64_t *prime_list   = NULL;   /* 0‑based array of primes       */
static size_t  prime_count    = 0;       /* number of primes stored       */
static uint64_t prime_limit_max = 0;     /* largest number sieved for primes */

/* Simple sieve of Eratosthenes up to 'limit' (inclusive) */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit_max)
        return;    /* already have enough primes */

    free(prime_list);
    prime_list = NULL;
    prime_count = 0;

    /* is_prime[i] == 1 ⇔ i is prime (0 ≤ i ≤ limit) */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime)
        exit(EXIT_FAILURE);

    for (uint64_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    uint64_t sqrt_lim = (uint64_t)floor(sqrt((double)limit));
    for (uint64_t p = 2; p <= sqrt_lim; ++p)
        if (is_prime[p])
            for (uint64_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    /* Count primes */
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++prime_count;

    /* Store primes in an array */
    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = i;

    free(is_prime);
    prime_limit_max = limit;
}

/* ---------- sigma and prefix sums ---------- */
static seq_t *sigma_table   = NULL;   /* sigma(k) for k = 0 .. max */
static seq_t *prefix_sigma  = NULL;   /* prefix sums of sigma     */
static uint64_t sigma_limit_max = 0;  /* largest number for which sigma known */

/* Compute sigma(k) for all 1 ≤ k ≤ max_value   */
static void ensure_sigma_upto(uint64_t max_value)
{
    if (max_value <= sigma_limit_max)
        return;    /* already computed */

    free(sigma_table);
    free(prefix_sigma);

    /* Allocate zeroed memory */
    sigma_table  = (seq_t *)calloc((size_t)(max_value + 1), sizeof(seq_t));
    prefix_sigma = (seq_t *)calloc((size_t)(max_value + 1), sizeof(seq_t));
    if (!sigma_table || !prefix_sigma)
        exit(EXIT_FAILURE);

    /* Initialize sigma: add each divisor d to all its multiples */
    for (uint64_t d = 1; d <= max_value; ++d)
        for (uint64_t m = d; m <= max_value; m += d)
            sigma_table[(size_t)m] += (seq_t)d;

    /* Build prefix sums */
    prefix_sigma[0] = 0;
    for (uint64_t i = 1; i <= max_value; ++i)
        prefix_sigma[(size_t)i] = prefix_sigma[(size_t)(i - 1)] +
                                   sigma_table[(size_t)i];

    sigma_limit_max = max_value;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, but avoid crash */

    /* Estimate an upper bound for the n-th prime */
    uint64_t limit;
    if (n < 6)
        limit = 15;
    else
    {
        double dn = (double)n;
        limit = (uint64_t)(dn * (log(dn) + log(log(dn))) + 3.0);
    }

    /* Ensure we have at least n primes */
    while (prime_count < (size_t)n)
    {
        ensure_primes_upto(limit);
        if (prime_count < (size_t)n)
            limit *= 2;      /* retry with a larger limit */
    }

    uint64_t prime_val = prime_list[(size_t)(n - 1)];   /* n-th prime, 1‑based index */

    /* Compute sigma and prefix sums up to this prime */
    ensure_sigma_upto(prime_val);

    return prefix_sigma[(size_t)prime_val];
}
