/*
 * A343071  Irregular triangle read by rows: T(n,k) = 2^(2n+1) * L(n,k), where
 *          L(n,k) is the k-th left endpoint after the n-th step of removal in
 *          the construction of the Smith-Volterra-Cantor set (SVC), 0 <= k <=
 *          2^n - 1.
 * https://oeis.org/A343071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 8927, u64 n <= 8927,
 *            u128 n <= 8927, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A343071 – Irregular triangle read by rows.
 * For row n (0‑based) and column k (0 ≤ k < 2^n) let 
 *    T(n,k) = ∑_{i=0}^{n-1} b_i · 4^i · (2^(n-i)+3)
 * where k = ∑_{i=0}^{n-1} b_i·2^i is the binary expansion of k.
 * The sequence a(m) is the flattened triangle: a(m) = T(n,k) for the
 * unique (n,k) with m = 2^n – 1 + k.
 */

#include <stdint.h>               /* for uint64_t typedefs */
#include "../utils/pow2.h"        /* seq_t pow2(idx_t exp)   */
#include <stdbool.h>              /* for bool                */

/* Computes a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* Determine the row index r such that 2^r - 1 <= n < 2^(r+1) - 1. */
    idx_t r = 0;
    /* n+1 is at most 2^64 for the given constraints, so shifting is safe. */
    while (((idx_t)1 << (r + 1)) <= n + (idx_t)1) {
        ++r;
    }

    /* Start index of row r: 2^r - 1. */
    idx_t start = ((idx_t)1 << r) - (idx_t)1;
    /* Column index inside the row. */
    idx_t k = n - start;

    seq_t result = 0;

    /* Iterate over the bits of k. */
    for (idx_t i = 0; i < r; ++i) {
        if (((k >> i) & 1ULL) == 0) {
            continue;
        }

        /* 4^i  = 2^(2i)          */
        seq_t p4 = pow2((idx_t)2 * i);
        /* (2^(r-i)+3)            */
        seq_t addend = pow2(r - i);
        addend += (seq_t)3;
        /* Contribution of this bit. */
        result += p4 * addend;
    }

    return result;
}
