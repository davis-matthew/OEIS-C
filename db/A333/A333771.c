/*
 * A333771  Triangular numbers that are the product of four distinct primes.
 * https://oeis.org/A333771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th triangular number that is the product of exactly
 *            four distinct primes (each with exponent one). This is computed
 *            by searching successive triangular numbers and testing the
 *            square‑free 4‑prime condition
 * Range:     u8 n <= 1, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C11 / C++17 compatible implementation */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/append_candidate.h"

/* Global state: prime sieve and cached results */
static uint64_t *primes    = NULL;   /* array of primes */
static size_t   prime_cnt  = 0;      /* number of primes obtained */
static seq_t  *cache       = NULL;   /* cached triangular numbers */
static size_t   cache_len  = 0;      /* number of cached entries    */
static size_t   cache_cap  = 0;      /* current allocation capacity */
static idx_t    last_m     = 0;      /* last m that was examined   */
static idx_t    matched    = 0;      /* number of terms found so far */

/* Build primes once, up to a limit large enough for all n considered
   (2·10⁶ gives >1 000 000 primes, comfortably covering n≈10⁴). */
static void
init_primes(void)
{
    if (primes != NULL)
        return;                       /* already initialized   */
    build_primes_upto(2000000, &primes, &prime_cnt);
}

/* Test if the m‑th triangular number is square‑free with exactly four
   distinct prime factors. If so, *result is set to T(m). */
static bool
good_triangular(idx_t m, seq_t *result)
{
    idx_t a = m;
    idx_t b = m + 1;

    /* Divide away one factor of 2 to keep a and b odd for factoring.  */
    if ((a & 1ULL) == 0)
        a >>= 1;
    else
        b >>= 1;

    size_t distinct = 0;

    /* Factor a. */
    idx_t x = a;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint64_t p = primes[i];
        if (x % p == 0) {
            distinct++;
            x /= p;
            if (x % p == 0)    /* exponent > 1 → not square‑free */
                return false;
        }
    }
    if (x > 1)          /* remaining prime factor */
        distinct++;

    /* Factor b. */
    x = b;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint64_t p = primes[i];
        if (x % p == 0) {
            distinct++;
            x /= p;
            if (x % p == 0)    /* exponent > 1 */
                return false;
        }
    }
    if (x > 1)
        distinct++;

    if (distinct != 4)
        return false;

    /* Compute the triangular number using a wider type before casting. */
    *result = (seq_t)((uint64_t)m * (m + 1) / 2);
    return true;
}

/* Public sequence function.  Computes a(n) for n ≥ 1.  Calls may be
   in any order; previously computed terms are cached. */
seq_t
a(idx_t n)
{
    if (n < 1)      /* undefined below offset; return 0 for safety  */
        return 0;

    init_primes();

    /* Generate terms until we have at least n of them. */
    while (matched < n) {
        last_m++;
        seq_t val;
        if (good_triangular(last_m, &val)) {
            append_candidate(&cache, &cache_len, &cache_cap, val);
            matched++;
        }
    }

    return cache[n - 1];
}
