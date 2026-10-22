/*
 * A314222  Coordination sequence Gal.6.643.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1; a(n) = 6*n - floor((n+2)/3) + 1, if n mod 12 ∈
 *            {7,8,10,11}; a(n) = 6*n - floor((n+2)/3), otherwise (for n > 0).
 *            This reproduces the OEIS sequence A314222
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)1;

    /* 6 * n  (performed in seq_t to keep compatibility) */
    seq_t prod = (seq_t)6 * (seq_t)n;

    /* floor((n+2)/3) computed with idx_t division then cast */
    seq_t sub = (seq_t)((n + (idx_t)2) / (idx_t)3);

    seq_t res = prod - sub;

    /* Adjust by +1 for the specific residues modulo 12 */
    idx_t mod12 = n % (idx_t)12;
    if (mod12 == (idx_t)7 || mod12 == (idx_t)8 ||
        mod12 == (idx_t)10 || mod12 == (idx_t)11)
        res += (seq_t)1;

    return res;
}
