/*
 * A360072  Number of pairs of positive integers (k,i) such that k >= i and
 *          there exists an integer partition of n of length k with i distinct
 *          parts.
 * https://oeis.org/A360072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) + Σ_{k=2}^{⌊(√(8n+1)-1)/2⌋} (1 + n - C(k+1,2)),
 *            where d(n) is the number of divisors of n and C(k+1,2) =
 *            (k+1)k/2. a(0) = 0
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_idx_t.h"

/* -------------------------------------------------- */
/* Integer square‑root for idx_t (n ≤ ~1e4 in practice) */
/* -------------------------------------------------- */
static idx_t
isqrt_idx_t(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* -------------------------------------------------- */
/* Count divisors of n (τ(n))                         */

/* -------------------------------------------------- */
/* Main sequence function                             */
/* -------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;        /* by definition */

    /* number of divisors of n */
    seq_t tau = divisor_count_idx_t(n);

    /* compute Kmax = floor((√(8n+1)-1)/2) */
    idx_t sq = isqrt_idx_t(1 + 8 * n);
    idx_t kmax = (sq > 0) ? (sq - 1) / 2 : 0;

    /* sum over k = 2 .. kmax */
    seq_t sum = 0;
    for (idx_t k = 2; k <= kmax; ++k) {
        seq_t binom = ((seq_t)(k + 1) * k) / 2;   /* C(k+1,2) */
        seq_t term = (seq_t)(n + 1) - binom;      /* 1 + n - binom */
        sum += term;
    }

    return tau + sum;
}
