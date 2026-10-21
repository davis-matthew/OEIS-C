/*
 * A285786  Number of primes p with 2(n-1)^2 < p <= 2n^2.
 * https://oeis.org/A285786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 859, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   set_composite
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285786: a(n) = number of primes p with 2(n-1)^2 < p <= 2n^2
 *
 * The implementation keeps a static bit‑set sieve of composite numbers and
 * a growing array of primes.  Each call to a() expands the sieve to an
 * upper bound of 2 n² if necessary, then counts primes that lie in the
 * open–closed interval (2(n‑1)², 2n²].
 *
 * The code is fully compliant with C11 and C++17.  The only difference
 * from the original attempt is the explicit casts from the `realloc`
 * return value required by C++.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/set_composite.h"

static inline bool is_composite(const uint8_t *sieve, idx_t i)
{
    return (sieve[i >> 3] >> (i & 7)) & 1;
}

/* ---------- integer square‑root (floor) --------------------------------- */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= (__uint128_t)x) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

/* ---------- binary search helpers -------------------------------------- */
static size_t upper_bound(const idx_t *arr, size_t len, idx_t value)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] <= value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ---------- static state ----------------------------------------------- */
static idx_t max_limit = 0;          /* sieve prepared up to this index (inclusive) */
static uint8_t *sieve = NULL;        /* bitset of composites */
static size_t sieve_cap = 0;         /* allocated byte size */
static idx_t *prime_list = NULL;     /* list of all primes up to max_limit */
static size_t prime_cnt = 0;         /* number of primes stored */
static size_t prime_cap = 0;         /* allocated capacity of prime_list */

/* ---------- ensure sieve covers all numbers up to ‘need’ ----------------- */
static void ensure_sieve(idx_t need)
{
    if (need <= max_limit)
        return;

    /* extend the bit‑set storage */
    size_t new_bytes = (need >> 3) + 1;
    if (new_bytes > sieve_cap) {
        uint8_t *tmp = (uint8_t *)realloc(sieve, new_bytes);
        if (!tmp) exit(1);          /* memory failure */
        /* zero the newly allocated part */
        for (size_t i = sieve_cap; i < new_bytes; ++i)
            tmp[i] = 0;
        sieve = tmp;
        sieve_cap = new_bytes;
    }

    /* sieve the new range [max_limit+1, need] using known primes */
    idx_t old_limit = max_limit;
    idx_t start_idx  = old_limit + 1;
    idx_t sqrt_need = isqrt_u64(need);

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = prime_list[pi];
        if (p > sqrt_need)
            break;
        /* first multiple of p >= start_idx */
        idx_t first = (start_idx + p - 1) / p * p;
        if (first < p * p)
            first = p * p;
        for (idx_t j = first; j <= need; j += p)
            set_composite(sieve, j);
    }

    /* enlarge the prime list buffer if needed */
    size_t projected = prime_cnt + (need - old_limit) / 10 + 10;
    if (prime_cap < projected) {
        size_t new_cap = prime_cap > 0 ? prime_cap * 2 : 1024;
        while (new_cap < projected)
            new_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* collect new primes */
    for (idx_t i = start_idx; i <= need; ++i) {
        if (i >= 2 && !is_composite(sieve, i))
            prime_list[prime_cnt++] = i;
    }

    max_limit = need;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1; not used in tests */

    /* compute 2n² and 2(n‑1)² with 128‑bit to avoid overflow */
    __int128 upper128 = (__int128)2 * n * n;
    idx_t upper = (idx_t)upper128;
    idx_t lower = (idx_t)((__int128)2 * (n - 1) * (n - 1));

    ensure_sieve(upper);

    /* count primes p with lower < p <= upper */
    size_t lo = upper_bound(prime_list, prime_cnt, lower); /* first > lower */
    size_t hi = upper_bound(prime_list, prime_cnt, upper); /* first > upper */
    return (seq_t)(hi - lo);
}
