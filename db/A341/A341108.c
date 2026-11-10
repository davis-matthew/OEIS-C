/*
 * A341108  a(n) = A163176(n+1)*A003557(n+1).
 * https://oeis.org/A341108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A163176(n+1)*A003557(n+1), where A163176(m) =
 *            A053657(m)/m! and A003557(m) = m / (largest squarefree divisor
 *            of m)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static size_t A163176_prime_count = 0;
static uint32_t *A163176_primes = NULL;
static bool *A163176_prime_mark = NULL;
static idx_t A163176_max_primes = 0;

static void A163176_ensure_primes(idx_t limit)
{
    if (limit <= A163176_max_primes)
        return;

    free(A163176_prime_mark);
    free(A163176_primes);
    A163176_prime_mark = NULL;
    A163176_primes = NULL;
    A163176_prime_count = 0;

    A163176_max_primes = limit;

    A163176_prime_mark = (bool *)malloc(sizeof(bool) * (A163176_max_primes + 1));
    if (!A163176_prime_mark)
        abort();

    for (idx_t i = 0; i <= A163176_max_primes; ++i)
        A163176_prime_mark[i] = true;
    A163176_prime_mark[0] = false;
    if (A163176_max_primes >= 1) A163176_prime_mark[1] = false;

    for (idx_t i = 2; i * i <= A163176_max_primes; ++i) {
        if (!A163176_prime_mark[i])
            continue;
        for (idx_t j = i * i; j <= A163176_max_primes; j += i)
            A163176_prime_mark[j] = false;
    }

    A163176_prime_count = 0;
    for (idx_t i = 2; i <= A163176_max_primes; ++i)
        if (A163176_prime_mark[i])
            ++A163176_prime_count;

    A163176_primes = (uint32_t *)malloc(sizeof(uint32_t) * (A163176_prime_count + 1));
    if (!A163176_primes)
        abort();

    size_t k = 0;
    for (idx_t i = 2; i <= A163176_max_primes; ++i)
        if (A163176_prime_mark[i])
            A163176_primes[k++] = (uint32_t)i;
}

static seq_t A163176_pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1)
            res *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return res;
}

static seq_t A163176_a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;

    A163176_ensure_primes(n);

    seq_t result = (seq_t)1;

    for (size_t idx = 0; idx < A163176_prime_count; ++idx) {
        uint32_t p = A163176_primes[idx];

        idx_t exp_mink = 0;
        idx_t denom = (idx_t)(p - 1);
        while (denom <= n - 1) {
            exp_mink += (n - 1) / denom;
            if (denom > (n - 1) / p)
                break;
            denom *= p;
        }

        idx_t exp_fact = 0;
        idx_t power = p;
        while (power <= n) {
            exp_fact += n / power;
            if (power > n / p)
                break;
            power *= p;
        }

        idx_t exponent = exp_mink - exp_fact;
        if (exponent > 0) {
            result *= A163176_pow_seq((seq_t)p, exponent);
        }
    }

    return result;
}

static seq_t A003557_a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; p++) {
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        for (idx_t i = 1; i < e; i++) {
            result *= (seq_t)p;
        }
    }

    return result;
}

seq_t a(idx_t n)
{
    return A163176_a(n + 1) * A003557_a(n + 1);
}
