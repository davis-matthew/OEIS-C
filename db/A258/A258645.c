/*
 * A258645  Fifth arithmetic derivative of n.
 * https://oeis.org/A258645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415^5(n)
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the 5th arithmetic derivative of n. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Arithmetic derivative of a 64‑bit integer. */
static seq_t d_uint64(uint64_t n) {
    if (n <= 1)
        return (seq_t)0;
    seq_t N = (seq_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);  /* decompose n */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t contrib = (seq_t)e * (N / (seq_t)p);
        sum += contrib;
    }
    return sum;
}

/* Fifth arithmetic derivative of n. */
seq_t a(idx_t n) {
    seq_t val = (seq_t)n;
    for (int i = 0; i < 5; ++i)
        val = d_uint64((uint64_t)val);
    return val;
}
