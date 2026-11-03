/*
 * A318983  a(n) = Sum_{d|n} Kronecker(-163, d).
 * https://oeis.org/A318983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-163, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 7798 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of Kronecker(-163, d) is done by a simple
   prime‑factorisation of d.  For an odd prime p ≠ 163 the factor
   contributes Jacobi(-163/p).  If the exponent of p in d is even
   the contribution is 1; if it is odd it is Jacobi(-163/p).  For
   the 2–part we use (−163|2^e) = (−1)^e.  When d is divisible by
   163 the symbol is 0. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Return Kronecker(-163, d).  Result is +1, -1, or 0. */
static int kronecker_minus163(uint64_t d)
{
    if (d == 0) return 0;  /* never used, but keep it safe */

    int ret = 1;          /* multiplicative identity           */
    int e2   = 0;         /* exponent of 2 in d                */

    /* Handle powers of 2 first */
    while ((d & 1ULL) == 0) {
        e2++;
        d >>= 1;
    }
    /* (−163|2) = −1, hence (−163|2^e) = (−1)^e  */
    if (e2 & 1) ret = -ret;

    if (d == 1) return ret;   /* no odd factors left */

    /* Factor the odd part */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(d, primes, exps, &cnt);  /* primes[].exps[] are exponents */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        if (p == 163) return 0;    /* common factor with –163 */

        /* Jacobi(-163/p) = Jacobi(-1/p) * Jacobi(163/p)  */
        int leg = jacobi_symbol(163, p);          /* Jacobi(163/p)          */
        if ((p & 3) == 3) leg = -leg;            /* multiply by Jacobi(-1/p) */

        if (e & 1) ret *= leg; /* odd exponent contributes, even exponent contributes 1 */
    }

    return ret;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    long long total = 0;               /* signed accumulation     */

    for (uint64_t d = 1; d <= nn / d; ++d) {
        if (nn % d == 0) {
            int k = kronecker_minus163(d);
            total += k;

            uint64_t d2 = nn / d;
            if (d2 != d) {
                k = kronecker_minus163(d2);
                total += k;
            }
        }
    }

    /* As all sequence values are non‑negative, the conversion is safe */
    return (seq_t)total;
}
