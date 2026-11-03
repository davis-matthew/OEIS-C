/*
 * A320775  a(n) is the least exponent k greater than 1 such that prime(n)^k
 *          starts and ends in prime(n).
 * https://oeis.org/A320775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least exponent k>1 such that prime(n)^k starts and
 *            ends in prime(n). This implementation computes k by - using
 *            modular arithmetic to check the trailing digits, - using
 *            fractional logarithms to check the leading digits, - exploiting
 *            the multiplicative order of prime(n) modulo 10^m (whe...
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 62 terms through n = 99 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/num_digits0.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* check whether the first m digits of p^k equal p, using the
   fractional part of k·log10(p). */
static inline bool leading_ok(uint64_t k, long double logp, long double lower,
                              long double upper, long double eps)
{
    long double f = fmodl(logp * (long double)k, 1.0L);
    return (f + eps >= lower) && (f < upper - eps);
}

/* modular exponentiation with 64‑bit arithmetic (works for all
   builds because p and mod fit into 64 bits for n ≤ 10000). */
static inline uint64_t modpow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑based). */
    uint32_t p32 = get_prime(n);
    uint64_t p   = p32;                      /* use unsigned 64‑bit for safety */

    /* Number of decimal digits of the prime. */
    int m = num_digits0((uint64_t)p32);
    if (m <= 0) m = 1;                       /* safety */

    /* Modulus 10^m used for trailing‑digit tests. */
    uint64_t mod = 1;
    for (int i = 0; i < m; ++i) mod *= 10ULL;

    /* Logarithmic bounds for leading‑digit test. */
    long double logp   = log10l((long double)p);
    long double lower  = log10l((long double)p)   - (m - 1);
    long double upper  = log10l((long double)(p + 1)) - (m - 1);
    const long double eps = 1e-12L;             /* small guard for rounding */

    /* Special handling for primes 2 and 5 (not coprime with 10). */
    if (p32 == 2U || p32 == 5U)
    {
        for (uint64_t k = 2ULL; ; ++k)
        {
            if (modpow_u64(p, k, mod) == p &&
                leading_ok(k, logp, lower, upper, eps))
                return (seq_t)k;
        }
    }

    /* Compute the multiplicative order r of p modulo 10^m. */
    uint64_t phi = 4ULL;                 /* φ(10^m) = 4·10^(m−1)    */
    for (int i = 1; i < m; ++i) phi *= 10ULL;
    uint64_t r = phi;

    /* Reduce r by prime factors 2 and 5. */
    while ((r % 2ULL == 0) && (modpow_u64(p, r / 2ULL, mod) == 1ULL))
        r /= 2ULL;
    while ((r % 5ULL == 0) && (modpow_u64(p, r / 5ULL, mod) == 1ULL))
        r /= 5ULL;

    /* Search for the first k = 1 + j·r (j ≥ 1) satisfying both tests. */
    for (uint64_t k = 1ULL + r; ; k += r)
    {
        if (modpow_u64(p, k, mod) == p &&
            leading_ok(k, logp, lower, upper, eps))
            return (seq_t)k;
    }
}
