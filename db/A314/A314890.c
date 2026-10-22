/*
 * A314890  Coordination sequence Gal.6.152.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For n≥1, a(n) = 5 + ⌊(n‑1)/11⌋·52 + Σ_{i=0}^{(n‑1)%11‑1}
 *            P[i], where P = [4,5,5,4,6,4,5,5,4,5,5] is the 11‑term periodic
 *            difference block. This yields sequence A314890
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;

    /* Periodic difference block after the first term */
    static const seq_t P[11] = {4,5,5,4,6,4,5,5,4,5,5};
    static const seq_t P_sum = 52; /* Sum of P */

    idx_t m    = n - 1;          /* Number of differences after the first */
    idx_t full = m / 11;         /* How many full periods */
    idx_t rem  = m % 11;         /* Remaining part of the period */

    seq_t result = 5;            /* 1 + initial difference 4 */

    result += (seq_t)full * P_sum;   /* Add complete periods */
    for (idx_t i = 0; i < rem; ++i) {
        result += P[i];               /* Add remainder */
    }
    return result;
}
