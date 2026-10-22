/*
 * A378221  Dirichlet inverse of sigma(A003961(n)), where A003961 is fully
 *          multiplicative function with a(prime(i)) = prime(i+1).
 * https://oeis.org/A378221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   multiplicative with a(p) = -q-1, a(p^2)=q, a(p^k)=0 for k>2,
 *            where q = next prime after p
 * Range:     i8 n <= 29, i16 n <= 2729, i32 n <= 9447, i64 n <= 9447,
 *            i128 n <= 9447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* global prime list for trial division */
static uint64_t *prime_list   = NULL;
static size_t    prime_count  = 0;
static idx_t     prime_limit  = 0;

/* integer square root for uint64_t */
static idx_t isqrt_u64(idx_t n)
{
    idx_t x = (idx_t)sqrt((long double)n);
    while (( (__int128)(x+1)*(x+1) ) <= n) ++x;
    while (( (__int128)x*x ) > n) --x;
    return x;
}

/* ensure prime_list contains all primes <= limit */
static void ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    free(prime_list);

    size_t sz = (size_t)limit + 1;
    bool *isprime = (bool*)calloc(sz, sizeof(bool));
    if (!isprime) exit(1);
    for (size_t i = 2; i < sz; ++i) isprime[i] = true;

    for (size_t i = 2; i * (size_t)i <= limit; ++i)
        if (isprime[i])
            for (size_t j = i * i; j <= limit; j += i)
                isprime[j] = false;

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (isprime[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (isprime[i]) prime_list[idx++] = (uint64_t)i;

    prime_count = cnt;
    prime_limit = limit;
    free(isprime);
}

/* trial‑division primality test */
static bool isPrimeSmall(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (( (__int128)p * p ) > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* first prime greater than start */
static uint64_t nextPrime(uint64_t start)
{
    uint64_t cand = start + 1;
    while (true) {
        idx_t lim = isqrt_u64(cand);
        ensure_prime_limit(lim);
        if (isPrimeSmall(cand))
            return cand;
        ++cand;
    }
}

/* exact sequence value */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = 1;
    uint64_t t = n;

    /* factorize n by primes up to sqrt(t) */
    idx_t sqrtn = isqrt_u64(t);
    ensure_prime_limit(sqrtn);

    for (size_t i = 0; i < prime_count && ( (__int128)prime_list[i] * prime_list[i] ) <= t; ++i) {
        uint64_t p = prime_list[i];
        if (t % p != 0) continue;

        int exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
            if (exp > 2) { /* exponent > 2 => a(n)=0 */
                result = 0;
                break;
            }
        }
        if (result == 0)
            break;

        uint64_t q = nextPrime(p);
        if (exp == 1) {
            seq_t factor = -((seq_t)q + 1);
            result *= factor;
        } else { /* exp == 2 */
            seq_t factor = (seq_t)q;
            result *= factor;
        }
    }

    if (result == 0)
        return result;

    /* remaining prime factor > sqrt original n */
    if (t > 1) {
        uint64_t p = t;
        uint64_t q = nextPrime(p);
        seq_t factor = -((seq_t)q + 1);
        result *= factor;
    }

    return result;
}
