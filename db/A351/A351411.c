/*
 * A351411  Number of divisors of n not of the form p^p, p prime.
 * https://oeis.org/A351411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) = number of divisors of n that are NOT of the form p^p (p prime).
   Formula: a(n) = τ(n) – #{p prime : p^p divides n}
   with τ(n) the divisor function.
*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factorise n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* τ(n) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (seq_t)(exps[i] + 1);
    }

    /* subtract prime‑power divisors p^p */
    static const uint64_t pp_vals[] = {
        4ULL,                /* 2^2   */
        27ULL,               /* 3^3   */
        3125ULL,             /* 5^5   */
        823543ULL,           /* 7^7   */
        285311670611ULL,     /* 11^11 */
        302875106592253ULL   /* 13^13 */
    };
    const size_t max_pp = sizeof(pp_vals) / sizeof(pp_vals[0]);

    size_t sub = 0;
    for (size_t i = 0; i < max_pp; ++i) {
        uint64_t pp = pp_vals[i];
        if (pp > (uint64_t)n) break;        /* remaining pp are larger */
        if ((uint64_t)n % pp == 0) ++sub;    /* pp is a divisor */
    }

    return tau - (seq_t)sub;
}
