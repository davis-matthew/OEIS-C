/*
 * A342421  a(n) = Sum_{k=1..n} (n/gcd(k,n))^(gcd(k,n) - 1).
 * https://oeis.org/A342421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * d^(n/d-1)
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 62, u64 n <= 122,
 *            u128 n <= 245, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

/* Max distinct prime factors for a 64‑bit n is < 16 */
#define MAX_PRIME_FACTORS 16

/* Simple trial division factorisation of n (uint64_t).
   Parameters:
     n     – the value to factor
     primes[ ] – array for distinct primes
     exps[ ]   – array for their exponents
     cnt   – zero‑initialised out‑param receiving number of factors.
*/
static void _factor_uint64(uint64_t n,
                           uint32_t *primes,
                           int *exps,
                           size_t *cnt)
{
    *cnt = 0;
    if (n <= 1ULL)
        return;

    /* factor 2 */
    if ((n & 1ULL) == 0ULL) {
        int e = 0;
        while ((n & 1ULL) == 0ULL) {
            n >>= 1;
            ++e;
        }
        primes[*cnt] = 2U;
        exps[*cnt] = e;
        ++(*cnt);
    }

    /* odd factors */
    uint64_t p = 3ULL;
    while (p <= n / p) {
        if (n % p == 0ULL) {
            int e = 0;
            while (n % p == 0ULL) {
                n /= p;
                ++e;
            }
            primes[*cnt] = (uint32_t)p;
            exps[*cnt] = e;
            ++(*cnt);
        }
        p += 2ULL;
    }

    if (n > 1ULL) {
        primes[*cnt] = (uint32_t)n;
        exps[*cnt] = 1;
        ++(*cnt);
    }
}

/* Recursive enumeration of all divisors of n.
   idx      : current prime index in factorisation
   curr_val : current constructed divisor d
   curr_phi : phi(d) computed so far
   primes[], exps[] : factorisation data
   cnt      : total number of distinct primes
   n        : original value (used to compute n/d)
   sum      : accumulating result
*/
static void _dfs_div(idx_t idx,
                     seq_t curr_val,
                     seq_t curr_phi,
                     const uint32_t *primes,
                     const int *exps,
                     size_t cnt,
                     idx_t n,
                     seq_t *sum)
{
    if (idx == cnt) {
        /* At a leaf: curr_val is a divisor d of n */
        idx_t div = n / (idx_t)curr_val;      /* n / d             */
        idx_t exp = div - (idx_t)1;           /* exponent for d^(n/d-1) */
        seq_t term = seq_pow(curr_val, exp);  /* d^(exp)            */
        term *= curr_phi;                    /* * phi(d)           */
        *sum += term;
        return;
    }

    uint32_t p = primes[idx];
    int maxe = exps[idx];

    for (int e = 0; e <= maxe; ++e) {
        /* build new divisor and corresponding phi */
        seq_t new_val = curr_val;
        seq_t new_phi = curr_phi;
        for (int k = 0; k < e; ++k) {
            new_val *= (seq_t)p;          /* multiply by p */
        }
        if (e > 0) {
            /* phi(p^e) = (p-1) * p^(e-1) */
            seq_t phi_pe = seq_pow((seq_t)p, (idx_t)(e - 1));
            phi_pe *= (seq_t)(p - 1);
            new_phi *= phi_pe;
        }
        _dfs_div(idx + 1, new_val, new_phi, primes, exps,
                 cnt, n, sum);
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint32_t primes[MAX_PRIME_FACTORS];
    int exps[MAX_PRIME_FACTORS];
    size_t cnt;

    _factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;
    _dfs_div(0, (seq_t)1, (seq_t)1, primes, exps, cnt, n, &sum);
    return sum;
}
