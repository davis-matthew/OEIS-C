/*
 * A320190  Number of integer solutions to a^2 + 2*b^2 + 3*c^2 + 9*d^2 = n.
 * https://oeis.org/A320190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (a,b,c,d ∈ ℤ) to a² + 2·b² +
 *            3·c² + 9·d² = n. Each non‑zero variable contributes a factor of
 *            2 for its two possible signs, whereas zero contributes a factor
 *            of 1. The function counts solutions for all integers
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n) {
    idx_t r = 0;
    /* Increment r until (r+1)² > n; use 128‑bit to avoid overflow. */
    while (( (__int128)(r + 1) * (r + 1) ) <= n) ++r;
    return r;
}

seq_t a(idx_t n) {
    seq_t result = 0;

    /* Loop over non‑negative a. */
    for (idx_t a = 0, a2 = 0; a <= isqrt_u64(n); ++a) {
        a2 = a * a;
        idx_t remA = n - a2;
        /* Loop over non‑negative b. */
        for (idx_t b = 0, b2 = 0; b <= isqrt_u64(remA / 2); ++b) {
            b2 = b * b;
            idx_t remAB = a2 + 2 * b2;
            /* Loop over non‑negative c. */
            for (idx_t c = 0, c2 = 0; c <= isqrt_u64((n - remAB) / 3); ++c) {
                c2 = c * c;
                idx_t remABC = remAB + 3 * c2;
                idx_t rem2 = n - remABC;
                /* rem2 must be non‑negative and divisible by 9. */
                if (rem2 == (idx_t)-1) continue; /* just to silence unused warnings */
                if (rem2 % 9 != 0) continue;
                idx_t t = rem2 / 9;             /* d² should equal t */
                idx_t d = isqrt_u64(t);
                if (d * d != t) continue;        /* not a perfect square  */
                /* Compute weight: 1 if variable is zero, else 2 (two signs). */
                seq_t weight = (seq_t)(
                    (a == 0 ? 1 : 2) *
                    (b == 0 ? 1 : 2) *
                    (c == 0 ? 1 : 2) *
                    (d == 0 ? 1 : 2)
                );
                result += weight;
            }
        }
    }

    return result;
}
