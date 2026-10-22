/*
 * A354974  Distance LQnR(n) (A334819) from n.
 * https://oeis.org/A354974
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - largest quadratic nonresidue modulo n (OEIS A354974)
 * Range:     u8 n <= 1326, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Helper: factor n into prime powers.  The arrays must have room for at least 32 factors. */
static void factorize_uint64(uint64_t n, uint64_t primes[32], int exps[32], int *cnt)
{
    int i = 0;
    /* Count factors of 2 */
    int e = 0;
    while ((n & 1ULL) == 0) {
        ++e;
        n >>= 1;
    }
    if (e > 0) {
        primes[i] = 2ULL;
        exps[i]   = e;
        ++i;
    }

    /* Odd primes */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[i] = p;
            exps[i]   = e;
            ++i;
        }
    }
    if (n > 1) {               /* remaining prime factor */
        primes[i] = n;
        exps[i]   = 1;
        ++i;
    }
    *cnt = i;
}

/* Test whether x is a quadratic nonresidue modulo modulus n.
   Uses the factorization of n and Legendre symbols. */
static bool is_nonresidue(uint64_t x, uint64_t n)
{
    uint64_t primes[32];
    int exps[32];
    int cnt, i;

    factorize_uint64(n, primes, exps, &cnt);

    for (i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];

        if (p == 2ULL) {
            if (e == 1) {                        /* n is even but only one factor of two – no special rule */
                continue;
            } else if (e == 2) {                 /* mod 4 */
                uint64_t r = x & 3ULL;
                if (r == 0 || r == 1)
                    continue;
                return true;
            } else {                              /* e >= 3 */
                uint64_t r = x & 7ULL;
                if (r == 0 || r == 1 || r == 4)
                    continue;
                return true;
            }
        } else {
            /* Count exponent of p dividing x */
            int v = 0;
            uint64_t tmp = x;
            while ((tmp % p) == 0) {
                tmp /= p;
                ++v;
            }
            if (v & 1)  /* odd exponent → nonresidue modulo p^e */
                return true;

            /* Even exponent – test Legendre symbol of remaining part modulo p */
            uint64_t a = tmp % p;
            seq_t leg = mod_pow((seq_t)a, (p - 1) / 2, (seq_t)p);
            if ((uint64_t)leg == (p - 1))   /* −1 modulo p → nonresidue */
                return true;
        }
    }
    return false;   /* residue modulo every prime power */
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn < 3)      /* undefined for n < 3; return 0 for safety */
        return (seq_t)0;

    for (uint64_t r = nn - 1; r >= 1; --r) {
        if (is_nonresidue(r, nn))
            return (seq_t)(nn - r);
    }
    return (seq_t)0; /* unreachable for n >= 3 */
}
