/*
 * A271418  Define a sequence by b(1) = n, b(m) = b(m-1) + b(m-1)/A052126(m) +
 *          1; then a(n) is the least m such that b(m) belongs to A270807.
 * https://oeis.org/A271418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, hear
 * Formula:   b(1)=n, b(m)=b(m-1)+A052126(b(m-1))+1; a(n)=least m with b(m) in
 *            A270807 (trajectory of 1 under x->x+x/gpf(x)+1); found by
 *            walking both increasing sequences in merge fashion
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9906 (2026-11-07)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

static uint64_t A271418_step(uint64_t x)
{
    uint64_t p = largest_prime_factor(x);
    return x + x / p + 1;
}

seq_t a(idx_t n)
{
    uint64_t b = (uint64_t)n;
    uint64_t t = 1;
    uint64_t m = 1;
    for (;;) {
        while (t < b) t = A271418_step(t);
        if (t == b) return (seq_t)m;
        b = A271418_step(b);
        m++;
    }
}
