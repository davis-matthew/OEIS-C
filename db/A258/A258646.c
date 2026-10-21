/*
 * A258646  Sixth arithmetic derivative of n.
 * https://oeis.org/A258646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415^6(n) (sixth iterate of the arithmetic derivative)
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t derivative(seq_t n) {
    if (n <= (seq_t)1)
        return (seq_t)0;
    /* n fits into uint64_t because the sequence starts at 0 and n ≤ 10000
       and subsequent derivatives stay below 2^64 for the required range.   */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)n / (seq_t)primes[i];
        term = term * (seq_t)exps[i];
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;
    for (int i = 0; i < 6; ++i)
        cur = derivative(cur);
    return cur;
}
