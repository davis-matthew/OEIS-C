/*
 * A321141  a(n) = Sum_{d|n} sigma_n(d).
 * https://oeis.org/A321141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma_n(d) = Sum_{d|n} d^n * tau(n/d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/pow_uint.h"

/* Recursive helper to accumulate the sum over all divisors of n. */
static void rec(size_t idx, seq_t cur_d, seq_t tau_rem, seq_t N,
                seq_t *sum, size_t pc, uint64_t *pr, uint64_t *exps)
{
    if (idx == pc) {
        seq_t term = big_pow(cur_d, N);
        term = term * tau_rem;
        *sum += term;
        return;
    }

    uint64_t p = pr[idx];
    uint64_t expmax = exps[idx];
    seq_t pow_pe = (seq_t)1;

    for (uint64_t e = 0; e <= expmax; ++e) {
        seq_t new_d  = cur_d * pow_pe;
        seq_t new_tau = tau_rem * ( (seq_t)(expmax - e + 1) );
        rec(idx + 1, new_d, new_tau, N, sum, pc, pr, exps);
        pow_pe = pow_pe * (seq_t)p;
    }
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* 1 is the first term (n=1).  All formulas work for n>=1. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pc = 0;

    /* Factorize n. */
    factor_u64((uint64_t)n, primes, exps, &pc);

    seq_t sum = (seq_t)0;
    seq_t N   = (seq_t)n;
    rec((size_t)0, (seq_t)1, (seq_t)1, N, &sum,
        pc, primes, exps);

    return sum;
}
