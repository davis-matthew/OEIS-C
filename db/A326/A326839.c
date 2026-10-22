/*
 * A326839  Numerator of A056239(n)/A061395(n) where A056239 is sum of prime
 *          indices and A061395 is maximum prime index.
 * https://oeis.org/A326839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of A056239(n)/A061395(n) where A056239(n) = ∑
 *            e_i * index(p_i) (index of p_i in the prime list) A061395(n) =
 *            max index(p_i) The function returns the numerator of the reduced
 *            fraction S / M with S = A056239(n) and M = A061395(n)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility header for gcd (seq_t compatible) */
#include "../utils/gcd.h"

/* --------------------------------------------------------------------- */
/* Integer square root for idx_t (unsigned 64‑bit)                         */
/* --------------------------------------------------------------------- */
static idx_t int_sqrt_uint64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* --------------------------------------------------------------------- */
/* Separated static data for prime sieve                                 */
/* --------------------------------------------------------------------- */
static uint64_t *primes      = NULL;   /* array of primes (2,3,5,…)       */
static size_t    primes_cnt  = 0;      /* number of primes stored        */
static idx_t     sieve_limit = 0;      /* current max limit that has been sieved */

/* --------------------------------------------------------------------- */
/* Ensure that all primes up to 'limit' are sieved into 'primes' array   */
/* --------------------------------------------------------------------- */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Free any previously allocated memory */
    free(primes);
    primes = NULL;
    primes_cnt = 0;

    /* Allocate and initialize sieve array */
    bool *is_prime = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_prime)
        return;  /* allocation failed – cannot compute further */

    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    idx_t sqrt_lim = int_sqrt_uint64(limit);
    for (idx_t i = 2; i <= sqrt_lim; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate primes array */
    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return;
    }

    /* Store primes into array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i])
            primes[idx++] = i;
    }
    primes_cnt = cnt;
    sieve_limit = limit;

    free(is_prime);
}

/* --------------------------------------------------------------------- */
/* Binary search for the 0‑based index of prime 'p' in the primes array   */
/* (prime 'p' must be present in the array).                             */
/* --------------------------------------------------------------------- */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo == hi; assuming 'p' is prime and present */
    return lo;   /* 0‑based index */
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; be defensive */
        return 0;
    if (n == 1)            /* a(1) = 0 by definition */
        return 0;

    /* Ensure we have all primes up to n for index look‑ups. */
    ensure_primes_upto(n);

    uint64_t tmp = (uint64_t)n;
    seq_t    S = 0;   /* sum of e * index(p) */
    seq_t    M = 0;   /* maximum prime index */

    /* Trial division using the prime list. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        unsigned __int128 sq = (unsigned __int128)p * p;
        if (sq > tmp)
            break;

        if (tmp % p == 0) {
            size_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            size_t idx = prime_index(p);   /* 0‑based */
            seq_t k = (seq_t)(idx + 1);    /* 1‑based prime index */
            S += (seq_t)exp * k;
            if (k > M)
                M = k;
        }
    }

    /* Remaining prime factor greater than sqrt(original n). */
    if (tmp > 1) {
        uint64_t p   = tmp;          /* prime */
        size_t   idx = prime_index(p);
        seq_t    k   = (seq_t)(idx + 1);

        S += k;   /* exponent == 1 */
        if (k > M)
            M = k;
    }

    /* If S==0 (should only happen when n==1) return 0. */
    if (S == 0)
        return 0;

    seq_t g = gcd(S, M);
    return S / g;
}
