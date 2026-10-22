/*
 * A394740  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are distinct primes and y < z.
 * https://oeis.org/A394740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of distinct primes with y<z
 *            satisfying x^2 + y*z = n. The function counts all such triples
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --- utilities for prime generation ----------------------------------- */

/* Simple sieve producing primes ≤ limit.
   The primes are stored in the static array `prime_list` and the count
   `prime_count`.  The array is reallocated only when a larger limit is
   requested. */
static uint64_t *prime_list = NULL;
static size_t     prime_count = 0;
static idx_t      prime_limit = 0;

static void prepare_primes(idx_t limit)
{
    if (prime_limit >= limit) return;          /* already sufficient */

    /* Allocate sieve array */
    uint8_t *sieve = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!sieve) return;                       /* out of memory → abort */

    size_t   i, j;
    for (i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (j = i * i; j <= limit; j += i)
                sieve[j] = 1;

    /* Count primes to allocate the list */
    size_t est = 0;
    for (i = 2; i <= limit; ++i)
        if (!sieve[i]) ++est;

    uint64_t *new_list = (uint64_t *)realloc(prime_list, est * sizeof(uint64_t));
    if (!new_list) { free(sieve); return; }
    prime_list = new_list;

    /* Fill list */
    size_t idx = 0;
    for (i = 2; i <= limit; ++i)
        if (!sieve[i]) prime_list[idx++] = i;
    prime_count = idx;
    prime_limit = limit;

    free(sieve);
}

/* --- integer square root ---------------------------------------------- */

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)n) return mid;
        if (sq < (unsigned __int128)n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* --- primality test using the existing prime list --------------------- */

static int is_prime_u64(idx_t v)
{
    if (v < 2) return 0;
    if (v == 2 || v == 3) return 1;
    if (v % 2 == 0) return 0;
    idx_t limit = isqrt_u64(v);
    for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i)
        if (v % prime_list[i] == 0) return 0;
    return 1;
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* trivial lower bound */

    /* Generate all primes up to floor(sqrt(n)) */
    idx_t limit = isqrt_u64(n);
    prepare_primes(limit);

    seq_t result = 0;

    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t x = prime_list[pi];
        idx_t sq = x * x;
        if (sq > n) break;

        idx_t m = n - sq;
        if (m <= 1) continue;

        /* Find the smallest prime factor y of m, if any. */
        idx_t y = 0;
        for (size_t pj = 0; pj < prime_count; ++pj) {
            idx_t p = prime_list[pj];
            if (p * p > m) break;
            if (m % p == 0) { y = p; break; }
        }
        if (y == 0) continue;           /* m is prime → cannot be product of two primes */

        if (y == x) continue;           /* y must differ from x */

        idx_t z = m / y;
        if (z <= 1 || z == y || z == x) continue;  /* distinctness */

        /* Check z is prime. */
        if (!is_prime_u64(z)) continue;

        /* Ensure y < z */
        if (y > z) { idx_t t = y; y = z; z = t; }

        if (y < z) ++result;
    }

    return result;
}
