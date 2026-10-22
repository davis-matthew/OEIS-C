/*
 * A342371  Partial sums of A051697.
 * https://oeis.org/A342371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2; a(n) = a(n-1) + A051697(n) for n≥1, where A051697(n)
 *            is the nearest prime to n (ties → smaller)
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Integer square root for idx_t (unsigned 64‑bit) */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* Prime list */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t   prime_limit = 0;

/* Build all primes up to "limit" with a simple sieve */
static void _ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                         /* already have enough */

    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    uint8_t *sieve = (uint8_t *)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!sieve)
        return;                         /* allocation failed */

    size_t i;
    for (i = 0; i <= (size_t)limit; ++i)
        sieve[i] = 1;
    if (limit >= 0)
        sieve[0] = 0;
    if (limit >= 1)
        sieve[1] = 0;

    idx_t sq = _isqrt_u64(limit);
    for (i = 2; i <= (size_t)sq; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (i = 2; i <= (size_t)limit; ++i)
        if (sieve[i])
            ++cnt;

    /* Allocate array */
    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) {
        free(sieve);
        return;
    }

    size_t pos = 0;
    for (i = 2; i <= (size_t)limit; ++i) {
        if (sieve[i])
            prime_list[pos++] = (uint64_t)i;
    }

    prime_cnt   = cnt;
    prime_limit = limit;
    free(sieve);
}

/* Nearest prime to n (ties → smaller) */
static seq_t _nearest_prime(idx_t n)
{
    /* Find largest prime ≤ n (lower) */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    seq_t lower;
    if (lo == 0)
        lower = (seq_t)2;
    else if (lo == prime_cnt)
        lower = (seq_t)prime_list[prime_cnt - 1];
    else
        lower = (seq_t)prime_list[lo - 1];

    /* Find smallest prime ≥ n (upper) */
    lo = 0; hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    seq_t upper;
    if (lo == prime_cnt)
        upper = (seq_t)prime_list[prime_cnt - 1];
    else
        upper = (seq_t)prime_list[lo];

    /* Compute distances (absolute) */
    idx_t diff_low  = (n >= (idx_t)lower) ? (n - (idx_t)lower) : ((idx_t)lower - n);
    idx_t diff_high = ((idx_t)upper >= n) ? ((idx_t)upper - n) : (n - (idx_t)upper);

    return (diff_low <= diff_high) ? lower : upper;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)2;

    /* Ensure primes are available up to a safe bound */
    idx_t limit = n + 2000;
    if (limit < 2000)
        limit = 2000;
    _ensure_primes(limit);

    seq_t sum = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i)
        sum = sum + _nearest_prime(i);

    return sum;
}
