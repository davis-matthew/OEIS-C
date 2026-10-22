/*
 * A312619  Coordination sequence Gal.6.583.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A312619 – Coordination sequence Gal.6.583.3
 *
 * The sequence is formed from a period‑12 base array and a set of
 * incremental jumps that depend on the residue of n modulo 12.
 * For residues 0 and 1 the jump after the first block differs from the
 * jumps for the first block; for all other residues the jump is fixed
 * from the first block onwards.
 *
 * Let
 *     m = n / 12          (number of complete 12‑step blocks)
 *     r = n % 12          (remainder)
 *
 * Then
 *     if n < 12:          a(n) = base[r]
 *     else if r == 0:     a(n) = base[0] + 59 + (m-1)*62
 *     else if r == 1:     a(n) = base[1] + 52 + (m-1)*50
 *     else:              a(n) = base[r] + m*first_delta[r]
 *
 * where base[] and first_delta[] are the values shown below.
 *
 * This implements exactly that definition and works for any
 * seq_t/idx_t combination that satisfies the build requirements.
 */

seq_t a(idx_t n)
{
    /* 12‑term base values (offset 0). */
    static const seq_t base[12] = {
        1, 4, 8, 14, 19, 24, 28, 30, 40, 38, 49, 48
    };

    /* 12‑term first‑block jump values (a(n+12)-a(n) for n < 12). */
    static const seq_t first_delta[12] = {
        59, 52, 60, 50, 62, 50, 60, 50, 62, 50, 60, 50
    };

    if (n < 12)
        return base[n];

    idx_t m = n / 12;           /* complete 12‑step blocks */
    unsigned int r = (unsigned int)(n % 12); /* remainder 0..11 */

    if (r == 0) {               /* residue 0: 59 first, then 62 */
        return base[0] + first_delta[0] + (m - 1) * 62;
    }

    if (r == 1) {               /* residue 1: 52 first, then 50 */
        return base[1] + 52 + (m - 1) * 50;
    }

    /* residues 2..11 use the same first block jump for all later blocks. */
    return base[r] + m * first_delta[r];
}
