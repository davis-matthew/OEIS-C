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
 * Helpers:   digit_count, get_prime, leading_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/get_prime.h"
#include "../utils/digit_count.h"
#include "../utils/leading_ok.h"

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
    int m = digit_count((uint64_t)p32);
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
