/*
 * A331388  a(n) = Sum_{k=1..n} mu(gcd(n, k)) * k / gcd(n, k).
 * https://oeis.org/A331388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(gcd(n,k)) * k / gcd(n,k)
 * Range:     u8 n <= 28, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   mobius_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                /* for uint64_t used in helper loops */
#include "../utils/mobius_idx.h"

/* Euclidean gcd for idx_t (uint64_t).  Works even when seq_t is 8‑bit. */
static inline idx_t gcd_u64(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t g = gcd_u64(n, k);                   /* gcd(n,k) */
        int mu = mobius_idx(g);                    /* μ(g) */
        if (mu == 0) continue;
        seq_t term = (seq_t)(k / g);               /* k / g  (integer) */
        if (mu > 0)
            sum += term;
        else if (mu < 0)
            sum -= term;
    }
    return sum;
}
