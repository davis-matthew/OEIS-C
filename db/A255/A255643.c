/*
 * A255643  Difference between sums of quadratic residues and non-residues
 *          modulo n that are coprime to n.
 * https://oeis.org/A255643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 31, i16 n <= 384, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A255643: difference between sums of quadratic residues
 * and non‑residues modulo n among integers coprime to n.
 * Algorithm: factor n, test each coprime k for being a residue
 * using Legendre symbols for odd primes and elementary rules for powers of 2.
 */
#include <stdint.h>
#include <stdbool.h>

/* greatest common divisor of two unsigned integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* modular exponentiation (base^exp mod mod) using 128‑bit intermediate. */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (uint64_t)((__int128)result * base % mod);
        exp >>= 1;
        base = (uint64_t)((__int128)base * base % mod);
    }
    return result;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* factor n into a, e pairs */
    uint64_t primes[32];
    int exps[32];
    int cnt = 0;
    uint64_t tmp = (uint64_t)n;

    if (tmp % 2ULL == 0ULL) {
        int e = 0;
        while (tmp % 2ULL == 0ULL) {
            tmp /= 2ULL;
            ++e;
        }
        primes[cnt] = 2ULL;
        exps[cnt++] = e;
    }
    for (uint64_t d = 3ULL; d * d <= tmp; d += 2ULL) {
        if (tmp % d == 0ULL) {
            int e = 0;
            while (tmp % d == 0ULL) {
                tmp /= d;
                ++e;
            }
            primes[cnt] = d;
            exps[cnt++] = e;
        }
    }
    if (tmp > 1ULL) {
        primes[cnt] = tmp;
        exps[cnt++] = 1;
    }

    seq_t diff = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (gcd_uint64((uint64_t)k, (uint64_t)n) != 1ULL)
            continue;

        bool is_res = true;
        for (int i = 0; i < cnt && is_res; ++i) {
            uint64_t p = primes[i];
            int e = exps[i];
            if (p == 2ULL) {
                if (e == 1) {
                    /* all odd k are residues modulo 2 */
                    /* nothing to check */
                } else if (e == 2) {
                    /* modulo 4: only 1 mod 4 is a residue */
                    if ((k & 3ULL) != 1ULL)
                        is_res = false;
                } else { /* e >= 3 */
                    /* modulo 2^e (e>=3): only 1 mod 8 is a residue */
                    if ((k & 7ULL) != 1ULL)
                        is_res = false;
                }
            } else {
                /* odd prime factor: use Legendre symbol */
                uint64_t leg = mod_pow_uint64((uint64_t)(k % p), (p - 1ULL) / 2ULL, p);
                if (leg != 1ULL)
                    is_res = false;
            }
        }

        if (is_res)
            diff += (seq_t)k;
        else
            diff -= (seq_t)k;
    }

    return diff;
}
