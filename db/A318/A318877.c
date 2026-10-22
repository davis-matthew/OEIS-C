/*
 * A318877  Sum of divisors d of n for which 2*phi(d) < d.
 * https://oeis.org/A318877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, 2*phi(d) < d} d
 * Range:     u8 n <= 119, u16 n <= 9207, u32 n <= 9207, u64 n <= 9207,
 *            u128 n <= 9207, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate all divisors of n, compute φ(d) for each, and sum those
   divisors that satisfy 2*φ(d) < d. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;                /* a(1) = 0 */

    /* Factor n into primes */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Enumerate divisors of n */
    seq_t divs[2048];                    /* 2048 >>> max divisor count for n ≤ 1e4 */
    size_t divs_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pk = primes[i];
        uint64_t exp = exps[i];
        size_t base_cnt = divs_cnt;
        seq_t pk_seq = (seq_t)pk;
        seq_t pk_pow = pk_seq;

        for (uint64_t e = 1; e <= exp; ++e) {
            for (size_t j = 0; j < base_cnt; ++j) {
                divs[divs_cnt++] = divs[j] * pk_pow;
            }
            pk_pow *= pk_seq;
        }
    }

    /* Sum qualifying divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < divs_cnt; ++i) {
        seq_t d = divs[i];
        if (d <= 1) continue;               /* d = 1 never satisfies the inequality */

        seq_t phi_d = phi((idx_t)d);        /* φ(d) */
        seq_t lhs   = phi_d + phi_d;        /* 2*φ(d) */

        if (lhs < d) sum += d;
    }

    return sum;
}
