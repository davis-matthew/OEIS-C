/*
 * A336352  Number of prime divisors of sigma(n) that are larger than the
 *          largest prime factor of n; a(1) = 0.
 * https://oeis.org/A336352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of distinct prime divisors of σ(n) that are larger than
 *            the largest prime factor of n; a(1)=0
 * Range:     u8 n <= 9835, u16 n <= 9835, u32 n <= 9835, u64 n <= 9835,
 *            u128 n <= 9835, big any n
 * Helpers:   factor_u64, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 0 */
    if (n < 1 || n == 1u) return (seq_t)0;

    /* Work with 64‑bit integers – n <= 10 000 for the required test
       range, so all intermediate values comfortably fit in uint64_t. */
    uint64_t N   = (uint64_t)n;
    uint64_t lpf = largest_prime_factor(N);   /* largest prime factor of n */

    /* ---- compute σ(n) --------------------------------------------- */
    uint64_t pf[64];
    uint64_t exp[64];
    size_t   cnt;
    factor_u64(N, pf, exp, &cnt);

    __uint128_t sigma = 1;                /* use 128‑bit to avoid overflow */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pf[i];
        uint64_t e   = exp[i];
        __uint128_t pow = 1;             /* p^(e+1)   */
        for (uint64_t k = 0; k <= e; ++k) pow *= p;
        __uint128_t term = (pow - 1) / (p - 1);   /* (p^(e+1)-1)/(p-1) */
        sigma *= term;
    }
    uint64_t sigma_val = (uint64_t)sigma;   /* fits in 64 bits for n ≤ 10 000 */

    /* ---- factor σ(n) and count primes > lpf ----------------------- */
    uint64_t spf[64];
    uint64_t sex[64];
    size_t   scnt;
    factor_u64(sigma_val, spf, sex, &scnt);

    seq_t count = 0;
    for (size_t i = 0; i < scnt; ++i) {
        if (spf[i] > lpf) {
            ++count;                     /* count distinct primes > lpf */
        }
    }

    return count;
}
