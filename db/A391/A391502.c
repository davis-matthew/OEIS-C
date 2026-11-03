/*
 * A391502  a(n) = Kronecker symbol (33/n).
 * https://oeis.org/A391502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker symbol (33/n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9272 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers used:
   - factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
   - jacobi_symbol(uint64_t a, uint64_t n)          */
/* The helper headers are expected to be in the same directory as the
   other utilities.                                  */
#include <stdint.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* The Kronecker symbol is 0 when the denominator is 0 or shares a
       prime factor with 33 (=3*11).                          */
    if (n == 0)
        return 0;

    uint64_t m = (uint64_t)n;

    if ((m % 3ULL) == 0 || (m % 11ULL) == 0)
        return 0;

    /* Factorise m. The array sizes are generous for the n values
       considered (n up to ~10 000).                         */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* (33/2) = 1 for all odd 33, so the prime 2 never changes
           the sign.                                              */
        if (p == 2)
            continue;

        /* Only an odd exponent can change the value because
           (a/p) = ±1 and (a/p^e) = (a/p)^e.                     */
        if ((e & 1ULL) == 0)
            continue;

        int s = jacobi_symbol(33ULL, p);   /* returns -1, 0 or 1 */

        if (s == 0)          /* should not happen after the gcd test */
            return 0;

        result *= (seq_t)s;
    }

    return result;
}
