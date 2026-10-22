/*
 * A377938  a(n) is the least k > n such that n is a primitive root modulo k,
 *          or -1 if there is no such k.
 * https://oeis.org/A377938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k > n such that n is a primitive root modulo k;
 *            returns -1 if no such k exists (happens exactly for squares > 1)
 * Range:     i8 n <= 118, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

/* Integer gcd for idx_t. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != (idx_t)0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Integer square root (floor) for uint64_t. */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            if (mid + 1 > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
    }
    return 0;
}

/* Modular exponentiation: computes (base^exp) mod mod efficiently. */
static inline idx_t mod_pow_idx(idx_t base, uint64_t exp, idx_t mod)
{
    idx_t result = 1 % mod;
    idx_t b = base % mod;
    while (exp != (uint64_t)0) {
        if (exp & 1)
            result = (idx_t)(((__uint128_t)result * b) % mod);
        b = (idx_t)(((__uint128_t)b * b) % mod);
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Return -1 for squares greater than 1. */
    if (n > 1) {
        idx_t s = isqrt_u64(n);
        if (s * s == n)
            return (seq_t)-1;
    }

    /* Temporary arrays for factorization. */
    uint64_t primes[64], exps[64];
    size_t cnt;

    /* Search for the least k > n. */
    for (idx_t k = n + 1; ; ++k) {
        /* Factor k. */
        factor_u64((uint64_t)k, primes, exps, &cnt);

        /* Determine if k admits a primitive root. */
        bool prime_root_ok = false;
        if (k == (idx_t)2 || k == (idx_t)4)
            prime_root_ok = true;
        else if (cnt == (size_t)1) {
            if (primes[0] == (uint64_t)2) {
                if (exps[0] <= (uint64_t)2)
                    prime_root_ok = true;
            } else
                prime_root_ok = true;
        } else if (cnt == (size_t)2) {
            if ((primes[0] == (uint64_t)2 && exps[0] == (uint64_t)1 && primes[1] != (uint64_t)2) ||
                (primes[1] == (uint64_t)2 && exps[1] == (uint64_t)1 && primes[0] != (uint64_t)2))
                prime_root_ok = true;
        }

        if (!prime_root_ok)
            continue;

        /* n and k must be coprime. */
        if (gcd_idx(n, k) != (idx_t)1)
            continue;

        /* Compute φ(k). */
        idx_t phi = k;
        for (size_t i = 0; i < cnt; ++i) {
            idx_t p = (idx_t)primes[i];
            phi = (phi / p) * (p - (idx_t)1);
        }

        /* Factor φ(k). */
        uint64_t phi_pr[64], phi_ex[64];
        size_t phi_cnt;
        factor_u64((uint64_t)phi, phi_pr, phi_ex, &phi_cnt);

        /* Test primitive root condition. */
        bool ok = true;
        for (size_t i = 0; i < phi_cnt; ++i) {
            uint64_t q = phi_pr[i];
            uint64_t exp = (uint64_t)phi / q;
            if (mod_pow_idx(n, exp, k) == (idx_t)1) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)k;
    }
}
