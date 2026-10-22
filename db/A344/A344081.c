/*
 * A344081  a(n) = Sum_{d|n} tau(d)^d, where tau(n) is the number of divisors
 *          of n.
 * https://oeis.org/A344081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} tau(d)^d
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/big_pow.h"

static void rec(size_t pos, seq_t d, seq_t tauProd, const uint64_t* primes,
                const uint64_t* exps, size_t k, seq_t* sum) {
    if (pos == k) {
        seq_t power = big_pow(tauProd, d);
        *sum = *sum + power;
        return;
    }
    uint64_t p = primes[pos];
    size_t emax = exps[pos];
    seq_t curPow = 1;
    for (size_t e = 0; e <= emax; ++e) {
        seq_t newD   = seq_mul(d, curPow);
        seq_t newTau = seq_mul(tauProd, (seq_t)(e + 1));
        rec(pos + 1, newD, newTau, primes, exps, k, sum);
        curPow = seq_mul(curPow, (seq_t)p);
    }
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);
    seq_t sum = 0;
    rec(0, (seq_t)1, (seq_t)1, primes, exps, pcnt, &sum);
    return sum;
}
