/*
 * A292919  Sum of n-th powers of odd divisors of n.
 * https://oeis.org/A292919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   Sum of n-th powers of odd divisors of n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 343 (2026-11-01)
 * Helpers:   ipow, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_u64_idx.h"

/* factorize m into primes[] and exps[], count in cnt */
static void factor_uint(uint64_t m, uint64_t primes[], int exps[], size_t *cnt)
{
    size_t idx = 0;
    uint64_t x = m;
    int e = 0;

    while ((x & 1ULL) == 0) {
        e++;
        x >>= 1;
    }
    if (e > 0) {
        primes[idx] = 2;
        exps[idx] = e;
        idx++;
    }

    for (uint64_t d = 3; d * d <= x; d += 2) {
        e = 0;
        while (x % d == 0) {
            e++;
            x /= d;
        }
        if (e > 0) {
            primes[idx] = d;
            exps[idx] = e;
            idx++;
        }
    }

    if (x > 1) {
        primes[idx] = x;
        exps[idx] = 1;
        idx++;
    }

    *cnt = idx;
}

/* Σ_{d|m} d^n_exp, where n_exp is the index exponent */
static seq_t sigma_for_n(uint64_t m, uint64_t n_exp)
{
    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factor_uint(m, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base   = ipow((seq_t)primes[i], (seq_t)n_exp);
        seq_t term   = 1;
        seq_t partial = 1;
        for (int k = 1; k <= exps[i]; ++k) {
            term    = term * base;
            partial = partial + term;
        }
        result = result * partial;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence is defined for n ≥ 1 */
    uint64_t nu = (uint64_t)n;

    seq_t sigma_n = sigma_for_n(nu, nu);          /* Σ_{d|n} d^n  */
    if ((nu & 1ULL) == 1) return sigma_n;        /* n odd → all divisors odd */

    seq_t sigma_half = sigma_for_n(nu / 2, nu);   /* Σ_{d|n/2} d^n   */
    seq_t pow2_n     = ipow_u64_idx(2, nu);           /* 2^n             */
    seq_t sub        = pow2_n * sigma_half;       /* contribution of even divisors */
    return sigma_n - sub;
}
