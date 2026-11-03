/*
 * A305054  If n = Product_i prime(x_i)^k_i, then a(n) = Sum_i k_i *
 *          omega(x_i), where omega = A001221 is number of distinct prime
 *          factors.
 * https://oeis.org/A305054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ k_i * ω(i), where n = Π prime(i)^k_i and ω(i) is the
 *            number of distinct prime factors of the integer i (the index of
 *            the prime)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Binary search avoiding overflow */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 1, hi = n >> 1;            /* hi < sqrt(max) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid to avoid infinite loop */
        if (mid > n / mid)                /* mid*mid > n   -> overflow or too big */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* The build supplies seq_t and idx_t; we rely on their definitions. */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) {
    /* Simple Eratosthenes sieve.  limit >= 2. */
    size_t i, j;
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    if (!sieve) return;             /* out of memory; return without primes */
    for (i = 2; i <= limit; ++i) sieve[i] = true;
    for (i = 2; i * i <= limit; ++i) if (sieve[i]) {
        for (j = i * i; j <= limit; j += i) sieve[j] = false;
    }

    /* Count primes. */
    *cnt_out = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) ++(*cnt_out);

    *primes_out = (uint64_t *)malloc(*cnt_out * sizeof(uint64_t));
    if (!*primes_out) {
        free(sieve);
        *cnt_out = 0;
        return;
    }
    size_t pos = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) (*primes_out)[pos++] = (uint64_t)i;

    free(sieve);
}

/* prime counting function pi(x) using a simple sieve */
static seq_t prime_pi(idx_t limit) {
    if (limit < 2) return 0;
    uint8_t *is_prime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) return 0;          /* out of memory → 0 as a safe fallback */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p) is_prime[q] = 0;
        }
    }
    seq_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++count;
    free(is_prime);
    return count;
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
    if (n < 1) return 0;          /* a(1) = 0, safe for other values */

    /* Static cache of primes up to the largest √n seen so far. */
    static uint64_t *primes = NULL;
    static size_t   prime_cnt = 0;
    static idx_t    current_limit = 0;

    idx_t sqrt_n = _floor_isqrt_u64(n);
    if (sqrt_n > current_limit)
    {
        free(primes);                     /* release old list if any */
        build_primes_upto(sqrt_n, &primes, &prime_cnt);
        current_limit = sqrt_n;
        if (!primes) return 0;            /* allocation failure fallback   */
    }

    idx_t temp = n;
    seq_t result = 0;

    /* Factorization via trial division using the cached primes. */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp) break;

        if (temp % p == 0)
        {
            seq_t exp = 0;
            do
            {
                temp /= p;
                ++exp;
            } while (temp % p == 0);

            idx_t idx = (idx_t)prime_pi(p);                     /* index of prime p */
            seq_t omega = (seq_t)count_distinct_primes((uint64_t)idx); /* ω(idx)  */
            result += exp * omega;
        }
    }

    /* Remaining prime factor > √n, if any. */
    if (temp > 1)
    {
        uint64_t p = temp;
        seq_t exp = 1;
        idx_t idx = (idx_t)prime_pi(p);
        seq_t omega = (seq_t)count_distinct_primes((uint64_t)idx);
        result += exp * omega;
    }

    return result;
}
