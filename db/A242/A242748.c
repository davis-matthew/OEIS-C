/*
 * A242748  Number of ordered ways to write n = k + m with 0 < k <= m such
 *          that k is a primitive root modulo prime(k) and m is a primitive
 *          root modulo prime(m).
 * https://oeis.org/A242748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (k,m) with 0 < k <= m, * k + m =
 *            n, and both k and m are primitive roots modulo * prime(k) and
 *            prime(m), respectively
 * Range:     u8 n <= 2195, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* cache of the first primes used by the algorithm */
static uint64_t *prime_list  = NULL;
static size_t   prime_count  = 0;
static bool     primes_ready = false;

/* Create a sieve for primes up to an estimate that covers the first
 * `idx_limit` primes.  This routine is called lazily from a() and
 * reallocates the cache when more primes are needed. */
static void
ensure_primes(size_t idx_limit)
{
    if (primes_ready && prime_count >= idx_limit)
        return;

    /* Upper bound for the idx_limit‑th prime: p_n < n(ln n + ln ln n)
     * for n >= 6.  The formula below is conservative and fast. */
    size_t limit = idx_limit * 20 + 100;
    bool *is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime)
        return;                 /* memory exhausted – nothing to do */

    for (size_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                is_prime[q] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint64_t *pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(is_prime);
        return;
    }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            pr[pos++] = i;

    free(is_prime);

    free(prime_list);
    prime_list   = pr;
    prime_count  = cnt;
    primes_ready = true;
}

/* fast modular exponentiation: base^exp mod mod (all 64‑bit) */
static seq_t
mod_pow_seq(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * cur % mod;
        cur = (unsigned __int128)cur * cur % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

/* test whether g is a primitive root modulo p (p is prime) */
static bool
is_primitive_root(uint64_t g, uint64_t p)
{
    if (g % p == 0)
        return false;
    if (p == 2)
        return (g % p == 1);

    uint64_t phi = p - 1;
    uint64_t temp = phi;

    for (size_t i = 0; i < prime_count &&
         (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i) {
        uint64_t q = prime_list[i];
        if (temp % q == 0) {
            if (mod_pow_seq(g, phi / q, p) == 1)
                return false;
            while (temp % q == 0)
                temp /= q;
        }
    }
    if (temp > 1) {
        if (mod_pow_seq(g, phi / temp, p) == 1)
            return false;
    }
    return true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* out of range for OEIS; simply return 0 */
        return 0;

    /* Ensure we have primes up to index n (and up to m = n-k). */
    if (!primes_ready || prime_count < (size_t)n)
        ensure_primes((size_t)n);

    seq_t count = (seq_t)0;
    for (idx_t k = 1; k <= n / 2; ++k) {
        idx_t m = n - k;
        if (k > m)  /* safety, though loop condition already guarantees it */
            break;

        uint64_t pk = prime_list[k - 1];   /* prime(k) */
        uint64_t pm = prime_list[m - 1];   /* prime(m) */

        if (is_primitive_root((uint64_t)k, pk) &&
            is_primitive_root((uint64_t)m, pm))
            ++count;
    }
    return count;
}
