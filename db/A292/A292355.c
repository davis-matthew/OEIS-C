/*
 * A292355  Number of distinct convex equilateral n-gons having rotational
 *          symmetry and with corner angles of m*Pi/n (0 < m <= n).
 * https://oeis.org/A292355
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -(1+(-1)^n)/2 + (1/n) * Σ_{d|n} ( φ(n/d) - μ(n/d) ) *
 *            C(3d-1, d-1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 53, u128 n <= 101,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS A292355 sequence. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static uint32_t primes[20000];
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Generate primes up to 'limit' (inclusive). 1 is not included. */
static void generate_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Start searching at the next integer after current limit. */
    for (idx_t i = prime_limit + 1; i <= limit; ++i) {
        if (i < 2)
            continue;
        bool is_prime = true;
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = primes[j];
            if ((uint64_t)p * p > i)
                break;
            if (i % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes[prime_count++] = (uint32_t)i;
    }
    prime_limit = limit;
}

/* Compute Euler's totient φ(m) for m <= prime_limit.  Assumes primes[] is filled up to m. */
static seq_t phi_u64(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t temp = m;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp)
            break;
        if (temp % p == 0) {
            result -= result / (seq_t)p;
            do {
                temp /= p;
            } while (temp % p == 0);
        }
    }
    if (temp > 1)
        result -= result / (seq_t)temp;
    return result;
}

/* Compute Möbius μ(m) for m <= prime_limit.  Result is -1, 0, or 1. */
static int8_t mobius_u64(idx_t m)
{
    int8_t mu = 1;
    idx_t temp = m;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp)
            break;
        int count = 0;
        while (temp % p == 0) {
            temp /= p;
            ++count;
        }
        if (count > 1)
            return 0;
        if (count == 1)
            mu = -mu;
    }
    if (temp > 1)
        mu = -mu;
    return mu;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* values below the offset are undefined */

    /* Ensure primes up to n are available for factorisation. */
    generate_primes_up_to(n);

    seq_t sum = 0;
    /* Iterate over all divisors d of n. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* First divisor: d */
        {
            idx_t nd = n / d;          /* n/d */
            seq_t phi_val = phi_u64(nd);
            int8_t mu_val = mobius_u64(nd);
            seq_t diff = phi_val - (seq_t)mu_val;          /* φ - μ */

            seq_t bin = binomial(3 * d - 1, d - 1);
            sum += diff * bin;
        }

        /* Second divisor: d2 = n/d, if distinct. */
        idx_t d2 = n / d;
        if (d2 == d)
            continue;

        {
            idx_t nd = n / d2;          /* n/d2 = d */
            seq_t phi_val = phi_u64(nd);
            int8_t mu_val = mobius_u64(nd);
            seq_t diff = phi_val - (seq_t)mu_val;          /* φ - μ */

            seq_t bin = binomial(3 * d2 - 1, d2 - 1);
            sum += diff * bin;
        }
    }

    /* Scale by 1/n.  The division is exact. */
    seq_t result = sum / (seq_t)n;

    /* Subtract 1 if n is even: -(1+(-1)^n)/2 = 0 for odd n, -1 for even n. */
    if ((n & 1) == 0)
        result -= (seq_t)1;

    return result;
}
