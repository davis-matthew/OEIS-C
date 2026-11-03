/*
 * A377379  a(n) = rad(n)^binomial(bigomega(n) + omega(n), omega(n) + 1),
 *          where rad = A007947, bigomega = A001222, and omega = A001221.
 * https://oeis.org/A377379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rad(n)^binomial(bigomega(n) + omega(n), omega(n) + 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 1543 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t rad(idx_t m)
{
    seq_t r = (seq_t)1;
    if (m < 2) return r;
    /* Handle factor 2 */
    if ((m & 1) == 0) {
        r *= (seq_t)2;
        while ((m & 1) == 0) m >>= 1;
    }
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            r *= (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) r *= (seq_t)m;  /* remaining prime factor */
    return r;
}

static seq_t bigomega(idx_t n)
{
    /* Count prime factors of n with multiplicity. */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    /* Count factors of 2. */
    while ((n & 1) == 0) {
        n >>= 1;
        ++count;
    }

    /* Test odd divisors up to sqrt(n). */
    for (idx_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            n /= p;
            ++count;
        }
    }

    /* If n > 1 now, it is a prime factor > sqrt(original n). */
    if (n > 1)
        ++count;

    return count;
}

/* Compute binomial coefficient n choose k multiplicatively.
   This works for any seq_t that supports + - * / % with integer constants. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)      /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Multiply-by-self exponentiation for big exponent (seq_t type) */
static seq_t big_pow(seq_t base, seq_t exp) {
    seq_t result = 1;
    while (exp != 0) {
        if ((exp % 2) != 0) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    /* radical of n */
    seq_t rad_val = rad(n);

    /* number of distinct prime factors (omega) */
    idx_t omega = (idx_t)count_distinct_primes(n);

    /* total number of prime factors counted with multiplicity (bigomega) */
    seq_t bigomega_val = bigomega(n);
    idx_t bigomega = (idx_t)bigomega_val;

    /* compute the binomial exponent */
    seq_t exp_binom = binomial(bigomega + omega, omega + 1);

    /* compute the result */
    seq_t result = big_pow(rad_val, exp_binom);

    return result;
}
