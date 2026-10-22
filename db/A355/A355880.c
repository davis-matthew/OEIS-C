/*
 * A355880  a(n) is a maximum of t*u*v such that 2*t*u + 2*t*v + 2*u*v <= n,
 *          where t,u,v are positive integers.
 * https://oeis.org/A355880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum of t*u*v such that 2*t*u + 2*t*v + 2*u*v <= n,
 *            t,u,v∈ℕ⁺
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the maximum volume of a rectangular cuboid with surface area ≤ n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t best = 0;
    seq_t N = (seq_t)n / 2;          /* Max allowed value for t*u + t*v + u*v */

    for (idx_t t = 1; ; ++t) {
        seq_t t1 = (seq_t)t;
        if (t1 * t1 > N) break;      /* t cannot exceed sqrt(N) */

        for (idx_t u = t; ; ++u) {   /* enforce t ≤ u to avoid duplicates */
            seq_t u1 = (seq_t)u;
            seq_t tu = t1 * u1;
            if (tu > N) break;       /* even with v=1 we exceed N */

            seq_t denom = t1 + u1;    /* t + u */
            seq_t rem   = N - tu;     /* remaining budget for t*v + u*v */

            seq_t v = rem / denom;    /* maximal integer v satisfying the inequality */
            if (v < u1) break;        /* further u will only reduce v */

            seq_t vol = t1 * u1 * v;
            if (vol > best) best = vol;
        }
    }
    return best;
}
