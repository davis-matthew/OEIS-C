/*
 * A256759  Nonpositive part of the minimal alternating triangular-number
 *          representation of n (defined at A255974).
 * https://oeis.org/A256759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nonpositive part of the minimal alternating
 *            triangular‑number representation of n
 * Range:     u8 n <= 279, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   min_k_at_least
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The representation is built recursively: the first positive term is the smallest triangular number ≥ n,
   the remainder is represented again with alternating signs starting with a positive term. */

#include <stdint.h>
#include "../utils/min_k_at_least.h"

struct Result { seq_t pos; seq_t neg; };

/* Recursively compute the sums of positive and negative terms of the alternating representation
   of n that starts with a positive sign and only uses indices ≤ max_k. */
static struct Result rep_sum(seq_t n, idx_t max_k) {
    if (n == 0) return (struct Result){0, 0};

    /* Find first positive term */
    idx_t k = 1;
    seq_t T = 0;
    for (k = 1; k <= max_k; ++k) {
        T = (seq_t)k * ((seq_t)k + 1) / 2;
        if (T >= n) break;
    }

    seq_t r = T - n;                       /* Remaining positive value to be represented alternately */
    struct Result inner = rep_sum(r, k - 1);

    struct Result res;
    res.pos = T + inner.neg;   /* Positive terms: T plus the former negative terms of inner */
    res.neg = inner.pos;       /* Negative terms: former positive terms of inner */
    return res;
}

/* Public function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety guard – sequence is 1‑indexed */
    seq_t N = (seq_t)n;                   /* convert idx_t to seq_t for arithmetic */

    idx_t k0 = min_k_at_least(N);
    struct Result res = rep_sum(N, k0);
    return res.neg;                       /* nonpositive part */
}
