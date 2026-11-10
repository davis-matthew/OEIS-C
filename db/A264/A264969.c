/*
 * A264969  Cycle length of n under bijection A264965 / A264966.
 * https://oeis.org/A264969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the cycle containing n under the permutation
 *            A264965(n) = A263273(A057889(n)); found by iterating until n
 *            recurs (a(n)=0 if no return within a step cap)
 * Range:     u8 n <= 30, u16 n <= 261, u32 n <= 261, u64 n <= 261,
 *            u128 n <= 261, big any n
 * Validated: standard, pass, 138 terms through n = 244 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A264969_adj_rev(seq_t n, unsigned base)
{
    if (n == (seq_t)0) return (seq_t)0;
    seq_t x = n;
    unsigned t = 0;
    while ((x % (seq_t)base) == (seq_t)0) { x /= (seq_t)base; ++t; }
    seq_t rev = (seq_t)0;
    while (x > (seq_t)0) {
        rev = rev * (seq_t)base + (x % (seq_t)base);
        x /= (seq_t)base;
    }
    for (unsigned i = 0; i < t; ++i) rev *= (seq_t)base;
    return rev;
}

static seq_t A264969_step(seq_t x)
{
    return A264969_adj_rev(A264969_adj_rev(x, 2), 3);
}

seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    if (n == 0) return (seq_t)1;
    seq_t x = A264969_step(start);
    seq_t len = (seq_t)1;
    while (x != start) {
        if (len > (seq_t)3000) return (seq_t)0; /* presumed infinite cycle */
        x = A264969_step(x);
        ++len;
    }
    return len;
}
