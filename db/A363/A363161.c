/*
 * A363161  Partial sums of A363031.
 * https://oeis.org/A363161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^n σ(6*k+1) where σ(m) is the sum of divisors of m
 * Range:     u8 n <= 8, u16 n <= 140, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* sum of divisors of a positive integer m. */
static seq_t sigma_of(uint64_t m) {
    seq_t res = 1;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            seq_t term = 1;
            seq_t power = 1;
            while (t % p == 0) {
                t /= p;
                power *= (seq_t)p;
                term += power;
            }
            res *= term;
        }
    }
    if (t > 1) {
        res *= ((seq_t)1 + (seq_t)t);
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        uint64_t m = 6ULL * k + 1;
        sum += sigma_of(m);
    }
    return sum;
}
