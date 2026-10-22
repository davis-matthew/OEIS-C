/*
 * A327670  Sum of divisors of n that have an even number of distinct prime
 *          factors.
 * https://oeis.org/A327670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of all positive divisors d of n that have an
 *            even number of distinct prime factors (including 0, which counts
 *            1)
 * Range:     u8 n <= 143, u16 n <= 9883, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool primes_initialized = false;
static uint32_t primes[1229];   /* primes <= 10000; there are 1229 of them */
static size_t prime_cnt = 0;

/* Build a list of all primes up to 10000 (once). */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    const size_t N = 10001;            /* 0 .. 10000 */
    bool *is_prime = (bool *)malloc(N);
    for (size_t i = 0; i < N; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i < N; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j < N; j += i)
                is_prime[j] = false;

    size_t k = 0;
    for (size_t i = 2; i < N; ++i)
        if (is_prime[i])
            primes[k++] = (uint32_t)i;
    prime_cnt = k;

    free(is_prime);
    primes_initialized = true;
}

/* Recursive helper that enumerates all divisors of a number given its
   prime factorization.  'idx' walks over the distinct primes.
   'cur' holds the product of the chosen exponents so far.
   'mask' remembers which primes have been used (bit i set if prime i
   appears with exponent ≥1).  When all primes have been processed (idx==m)
   we add 'cur' to *sum if the number of set bits in 'mask' is even.  */
static void recurse_divisors(size_t idx, seq_t cur, unsigned long long mask,
                             const uint64_t *primes_arr, const int *exps_arr,
                             size_t m, seq_t *sum)
{
    if (idx == m) {
        if (((__builtin_popcountll(mask) & 1) == 0))
            *sum += cur;
        return;
    }

    const uint64_t p = primes_arr[idx];
    seq_t val = cur;

    /* exponent 0 (skip this prime) */
    recurse_divisors(idx + 1, val, mask, primes_arr, exps_arr, m, sum);

    /* exponents 1 .. exps_arr[idx] */
    for (int e = 1; e <= exps_arr[idx]; ++e) {
        val *= (seq_t)p;
        recurse_divisors(idx + 1, val, mask | (1ULL << idx),
                         primes_arr, exps_arr, m, sum);
    }
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n=0; guard */

    init_primes();              /* guarantee we have the primes list */

    /* Factor n into distinct primes and their exponents. */
    uint64_t pf[64];
    int     exp[64];
    size_t  m = 0;
    uint64_t temp = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            int c = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++c;
            }
            pf[m]   = primes[i];
            exp[m++] = c;
        }
    }
    if (temp > 1) {            /* remaining prime factor > sqrt(n) */
        pf[m]   = temp;
        exp[m++] = 1;
    }

    /* Enumerate divisors and accumulate those with even distinct prime count. */
    seq_t sum = 0;
    recurse_divisors(0, (seq_t)1, 0ULL, pf, exp, m, &sum);
    return sum;
}
