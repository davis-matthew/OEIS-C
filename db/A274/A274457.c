/*
 * A274457  Shortest possible antipower period of a binary string of length n.
 * https://oeis.org/A274457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor d of n such that n/d <= 2^d
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test if divisor d satisfies n/d <= 2^d */
static bool satisfies(idx_t n, idx_t d) {
    /* 2^d grows very fast; for d>=64 the inequality always holds
       for any n that fits in a 64‑bit integer. */
    if (d >= 64) return true;
    /* Compute both sides in 128‑bit to avoid overflow, even if
       seq_t may be smaller than 128 bits. */
    unsigned __int128 rhs = ((unsigned __int128)1) << d;
    unsigned __int128 lhs = (unsigned __int128)(n / d);
    return lhs <= rhs;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is guaranteed >=1 by the OEIS offset */
    seq_t best = (seq_t)n;  /* at least n itself satisfies the condition */
    /* Enumerate divisors up to sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (satisfies(n, d) && (seq_t)d < best) best = (seq_t)d;
            /* paired divisor n/d */
            idx_t p = n / d;
            if (p != d && satisfies(n, p) && (seq_t)p < best) best = (seq_t)p;
        }
    }
    return best;
}
