/*
 * A338490  Sum of indices of distinct odd prime factors of n.
 * https://oeis.org/A338490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p odd prime} A000720(p)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* static data for prime generation */
static idx_t   *prime_list = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static idx_t    prime_lim   = 0;

/* generate all primes ≤ limit */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_lim) return;

    /* allocate sieve array */
    size_t sz = (size_t)(limit + 1);
    unsigned char *comp = (unsigned char*)calloc(sz, sizeof(unsigned char));
    if (!comp) abort();

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                comp[j] = 1;
        }
    }

    /* count primes and allocate storage if needed */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i]) ++cnt;

    if (prime_cap < cnt) {
        if (prime_list) free(prime_list);
        prime_list = (idx_t*)malloc(cnt * sizeof(idx_t));
        if (!prime_list) abort();
        prime_cap = cnt;
    }

    /* copy primes */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i]) prime_list[idx++] = i;

    prime_cnt = cnt;
    prime_lim = limit;
    free(comp);
}

/* binary search for the 1‑based index of prime p in prime_list */
static size_t prime_index(idx_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        idx_t val = prime_list[mid];
        if (val == p) return mid + 1;        /* found, indices start at 1 */
        if (val < p)  lo = mid + 1;
        else          hi = mid;
    }
    return 0; /* should not happen for a prime */
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    ensure_primes_up_to(n);

    seq_t sum = 0;
    uint64_t temp = (uint64_t)n;

    /* remove factor 2 */
    while ((temp & 1ULL) == 0) temp >>= 1;

    /* factor using the prime table */
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= temp / prime_list[i]; ++i) {
        idx_t p = prime_list[i];
        if (p == 2) continue;                 /* skip prime 2 */
        if (temp % p == 0) {
            sum += (seq_t)prime_index(p);      /* add index of this odd prime */
            while (temp % p == 0) temp /= p;   /* strip all powers of p */
        }
    }

    /* possible remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        if (temp != 2)                    /* ignore 2 if present */
            sum += (seq_t)prime_index(temp);
    }

    return sum;
}
