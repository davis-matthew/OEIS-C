/*
 * A350693  Number of b > 0 which permit n^3 to be written as a sum of powers
 *          of b in n parts. Each exponent c is an integer >= 0, n^3 = b^c_1 +
 *          b^c_2 + ... + b^c_n.
 * https://oeis.org/A350693
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of bases b > 1 such that n^3 can be written as a
 *            sum of exactly n powers of b. For each divisor d of n^3-n set
 *            b=d+1. Compute S = sum of digits of n^3 in base b. Then a base
 *            is valid iff S <= n and (n - S) is divisible by (b-1)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* internal helper structures and variables for prime sieve */
static bool primes_sieved = false;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

static inline void
sieve_primes(void)
{
    if (primes_sieved)
        return;
    const size_t LIM = 1000000; /* sqrt(10^12) ~ 10^6 */
    bool *is_prime = (bool *)malloc((LIM + 1) * sizeof(bool));
    for (size_t i = 0; i <= LIM; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIM; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIM; j += i)
                is_prime[j] = false;
        }
    }
    /* count primes to allocate array */
    prime_cnt = 0;
    for (size_t i = 2; i <= LIM; ++i)
        if (is_prime[i]) ++prime_cnt;
    primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIM; ++i) {
        if (is_prime[i])
            primes[idx++] = i;
    }
    free(is_prime);
    primes_sieved = true;
}

/* compute sum of digits of x in base 'base' */
static uint64_t
digit_sum(uint64_t x, uint64_t base)
{
    uint64_t s = 0;
    while (x) {
        s += x % base;
        x /= base;
    }
    return s;
}

/* recursive generation of divisors */
static void
dfs_divisors(const uint64_t *factors, const int *exps, int ftot,
             uint64_t cur, uint64_t n, uint64_t n3,
             uint64_t *cnt)
{
    if (ftot == 0) {
        uint64_t b_minus1 = cur;                     /* d = b-1 */
        uint64_t b = b_minus1 + 1;
        uint64_t s = digit_sum(n3, b);
        if (s <= n && ((n - s) % b_minus1) == 0)
            (*cnt)++;
        return;
    }
    uint64_t p = factors[0];
    uint64_t p_pow = 1;
    for (int e = 0; e <= exps[0]; ++e) {
        dfs_divisors(factors + 1, exps + 1, ftot - 1,
                     cur * p_pow, n, n3, cnt);
        p_pow *= p;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    uint64_t n64 = n;
    uint64_t n3 = n64 * n64 * n64;          /* n^3 fits in 64 bits (n <= 10^4) */
    uint64_t x = n3 - n64;                  /* n^3 - n */
    if (x == 0)
        return 0;

    sieve_primes(); /* ensure primes available */

    /* factorization of x */
    uint64_t temp = x;
    uint64_t factor_primes[16];
    int factor_exps[16];
    int factor_cnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            factor_primes[factor_cnt] = p;
            factor_exps[factor_cnt] = e;
            ++factor_cnt;
        }
    }
    if (temp > 1) {
        factor_primes[factor_cnt] = temp;
        factor_exps[factor_cnt] = 1;
        ++factor_cnt;
    }

    uint64_t count = 0;
    dfs_divisors(factor_primes, factor_exps, factor_cnt, 1, n64, n3, &count);

    return (seq_t)count;
}
