/*
 * A260675  Expansion of psi(x^2) * phi(x^15) in powers of x where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A260675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to n(n+1)+15k² = n, counted
 *            with * multiplicity 2 for k≠0 (k∈ℤ) and 1 for k=0. Equivalently,
 *            * a(n) = #{(u,v)∈ℕ×ℤ : u(u+1)+15v² = n}, with each non‑zero v
 *            counted twice
 * Range:     u8 n <= 754, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 103 terms through n = 2358 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return the n‑th term of OEIS sequence A260675. */
seq_t a(idx_t n) {
    seq_t count = 0;
    /* iterate over all u such that u(u+1) ≤ n */
    for (idx_t u = 0; (seq_t)u * ((seq_t)u + 1) <= (seq_t)n; ++u) {
        seq_t diff = (seq_t)n - (seq_t)u * ((seq_t)u + 1);
        if (diff % (seq_t)15 == 0) {
            seq_t r = diff / (seq_t)15;            /* 15v² = r */
            seq_t s = floor_sqrt(r);               /* floor(sqrt(r)) */
            if (s * s == r) {                      /* r is a perfect square */
                if (s == 0)
                    count += 1;                    /* v = 0 counted once */
                else
                    count += 2;                    /* v ≠ 0 counted twice */
            }
        }
    }
    return count;
}
