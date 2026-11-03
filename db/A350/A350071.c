/*
 * A350071  a(n) = gcd(sigma(n^2), A003961(n^2)), where A003961 shifts the
 *          prime factorization of n one step towards larger primes, and sigma
 *          is the sum of divisors function.
 * https://oeis.org/A350071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n^2), A003961(n^2))
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 5593, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9879 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_idx, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;

    /* factor n: pr[i]^ex[i]  */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(nn, pr, ex, &cnt);

    seq_t sigma_val = (seq_t)1;   /* sigma(n^2)   */
    seq_t shift_val = (seq_t)1;   /* A003961(n^2) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t exp = ex[i];            /* exponent of p in n */

        /* sigma contribution: (p^(2*exp+1)-1)/(p-1)  */
        seq_t base       = (seq_t)p;
        idx_t sigma_exp  = (idx_t)(2 * exp + 1);
        seq_t num        = ipow_idx(base, sigma_exp) - (seq_t)1;
        seq_t denom      = base - (seq_t)1;
        seq_t sigma_pe2  = num / denom;
        sigma_val *= sigma_pe2;

        /* A003961 contribution: nextprime(p)^(2*exp)  */
        uint64_t nextp    = next_prime(p);
        seq_t next_base   = (seq_t)nextp;
        idx_t shift_exp   = (idx_t)(2 * exp);
        seq_t shift_pe    = ipow_idx(next_base, shift_exp);
        shift_val *= shift_pe;
    }

    return gcd(sigma_val, shift_val);
}
