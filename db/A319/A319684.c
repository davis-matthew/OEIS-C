/*
 * A319684  Sum of A003415(d) over the divisors d of n, where A003415 is
 *          arithmetic derivative.
 * https://oeis.org/A319684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003415(d) Implementation of a(n) using prime
 *            factorisation. A003415(d) = d * sum_{p|d} (e_p / p) where e_p is
 *            the exponent of prime p in d
 * Range:     u8 n <= 31, u16 n <= 5759, u32 n <= 9251, u64 n <= 9251,
 *            u128 n <= 9251, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* arrays to keep S_i and T_i for each prime */
    seq_t S[64];
    seq_t T[64];

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t a = exps[i];
        seq_t sum = 0;
        seq_t sumk = 0;
        seq_t pow = 1;

        for (uint64_t k = 0; k <= a; ++k) {
            sum += pow;                /* S_i = Σ p^k */
            if (k > 0) sumk += (seq_t)k * pow;  /* T_i = Σ k * p^k */
            pow *= (seq_t)p;
        }
        S[i] = sum;
        T[i] = sumk;
    }

    /* total_S = product of all S_i */
    seq_t total_S = 1;
    for (size_t i = 0; i < cnt; ++i) {
        total_S *= S[i];
    }

    /* compute a(n) = Σ_i (T_i / p_i) * (total_S / S_i) */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t contrib = T[i] / (seq_t)primes[i];
        seq_t part   = total_S / S[i];
        res += contrib * part;
    }
    return res;
}
