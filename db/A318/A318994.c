/*
 * A318994  Totally additive with a(prime(n)) = n + 1.
 * https://oeis.org/A318994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^e‖n} e * (index(p)+1+1), where index(p) is the
 *            0‑based index of p in the ordered list of primes. Thus
 *            a(prime_k) = k+1. The function is totally additive:
 *            a(mn)=a(m)+a(n)
 * Range:     u8 n <= 1612, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t prime_limit = 0;

/* construct a simple sieve up to limit and store primes in primes[] */
static void grow_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    idx_t size = (limit > 2) ? limit + 1 : 3;
    bool *sieve = (bool *)malloc((size_t)size);
    if (!sieve) return;
    for (idx_t i = 0; i < size; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    uint64_t *newpr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!newpr) { free(sieve); return; }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i]) newpr[idx++] = (uint64_t)i;

    free(sieve);
    free(primes);
    primes = newpr;
    primes_cnt = cnt;
    prime_limit = limit;
}

/* binary search for a prime in the primes array, returns 0‑based index */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return primes_cnt; /* not found */
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    if (n > prime_limit) grow_primes(n);

    seq_t value = 0;
    idx_t m = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p * (idx_t)p > m) break;
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            value += (seq_t)exp * ((seq_t)i + 2);
        }
    }

    if (m > 1) {
        /* m is a prime factor larger than sqrt(original n) */
        size_t idx = prime_index((uint64_t)m);
        if (idx < primes_cnt)
            value += (seq_t)(idx + 2);
    }

    return value;
}
