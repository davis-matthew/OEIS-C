/*
 * A380344  Product of prime indices minus sum of prime factors of n.
 * https://oeis.org/A380344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (product of prime indices of n) – (sum of prime factors
 *            of n)
 * Range:     i8 n <= 61, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Prime table maintenance ---------- */
static uint64_t *prime_list = NULL; /* sorted prime list */
static size_t   prime_cnt  = 0;     /* number of primes in the list */
static idx_t    prime_limit = 0;    /* largest number for which the list is valid */

/* Ensure that prime_list contains all primes ≤ limit */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* discard old list */
    free(prime_list);
    prime_list = NULL;
    prime_cnt = 0;
    prime_limit = limit;

    /* simple sieve of Eratosthenes up to 'limit' */
    unsigned char *is_comp = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_comp) return;          /* out of memory – caller will fail gracefully */
    memset(is_comp, 0, (size_t)(limit + 1));

    for (idx_t i = 2; i <= limit / i; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* count primes to allocate the list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (!is_comp[i]) ++cnt;

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) { free(is_comp); return; }

    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) if (!is_comp[i]) prime_list[pos++] = (uint64_t)i;

    prime_cnt  = cnt;
    free(is_comp);
}

/* Find 0‑based index of a prime p in prime_list. p must be prime and present. */
static size_t prime_index_0based(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return lo;          /* primes[mid] == p */
}

/* ---------- Sequence computation ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* below offset – undefined in OEIS */

    /* 1 is a special case (empty prime factor multiset) */
    if (n == 1) return (seq_t)1;

    /* Ensure the prime table covers at least 'n' (needed for remaining prime factor). */
    ensure_primes_upto(n);

    /* Use 64‑bit intermediates to avoid overflow during accumulation. */
    int64_t prod64 = 1;
    int64_t sum64  = 0;

    uint64_t temp = n;

    /* Factorise by trial division using the prime table */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            size_t idx = prime_index_0based(p) + 1;   /* 1‑based index */
            for (int k = 0; k < exp; ++k) prod64 *= (int64_t)idx;
            sum64 += (int64_t)p * exp;
        }
    }

    /* Remaining factor > 1 is prime (if any) */
    if (temp > 1) {
        uint64_t p = temp;
        size_t idx = prime_index_0based(p) + 1;
        prod64 *= (int64_t)idx;
        sum64 += (int64_t)p;
    }

    /* Convert to seq_t, respecting possible arbitrary precision */
    seq_t prod = (seq_t)prod64;
    seq_t sum  = (seq_t)sum64;
    return prod - sum;
}
