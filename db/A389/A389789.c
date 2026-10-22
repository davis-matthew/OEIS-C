/*
 * A389789  Number of ways to write n as (2-(n mod 2))*p + q + q', where p and
 *          q are primes, and q' is the first prime after q.
 * https://oeis.org/A389789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (p,q) such that n = (2-(n mod 2))*p + q +
 *            q', where p and q are primes and q' is the first prime after q.
 *            The implementation builds a sieve of primes up to the largest n
 *            seen and a lookup table for all sums of two consecutive primes.
 *            Subsequent calls reuse the cached tables
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* static state */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_cnt    = 0;        /* number of primes */
static idx_t    prime_limit  = 0;        /* highest number sieved */

static uint8_t *sum_bool      = NULL;   /* sum_bool[x] = 1 if x = q+q' for consecutive primes */
static idx_t    sum_limit     = 0;       /* highest x for which sum_bool is valid */

/* ------------------------------------------------------------------------
   Sieve of Eratosthenes: generate all primes <= limit.
   ------------------------------------------------------------------------ */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate boolean array for primality tests */
    size_t sz = (size_t)(limit + 1);
    char *is_prime = (char*)malloc(sz);
    if (!is_prime) return;  /* allocation failure */

    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!list) { free(is_prime); return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) list[idx++] = (uint64_t)i;

    free(is_prime);
    free(prime_list);
    prime_list  = list;
    prime_cnt   = cnt;
    prime_limit = limit;
}

/* ------------------------------------------------------------------------
   Build a lookup table for all sums of two consecutive primes up to limit.
   ------------------------------------------------------------------------ */
static void
ensure_sum_bool(idx_t limit)
{
    if (limit <= sum_limit) return;

    free(sum_bool);
    sum_bool = (uint8_t*)calloc((size_t)(limit + 1), 1);
    if (!sum_bool) { sum_limit = 0; return; }

    if (limit > prime_limit) ensure_primes(limit);

    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        idx_t sum = prime_list[i] + prime_list[i + 1];
        if (sum > limit) break;
        sum_bool[(size_t)sum] = 1;
    }

    sum_limit = limit;
}

/* ------------------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n=0 – just return 0 */

    /* Build tables up to n */
    ensure_primes(n);
    ensure_sum_bool(n);

    const idx_t coeff  = (n & 1ULL) ? 1 : 2;        /* 2-(n mod 2) */
    const idx_t max_p  = n / coeff;
    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > max_p) break;

        idx_t rem = n - coeff * (idx_t)p;
        if (rem == 0) continue;                         /* cannot be a sum of two primes */
        if (rem <= sum_limit && sum_bool[(size_t)rem])
            ++count;
    }

    return count;
}
