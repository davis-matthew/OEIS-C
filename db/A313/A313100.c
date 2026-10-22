/*
 * A313100  Coordination sequence Gal.5.100.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n≥2, a(n)=4+∑_{k=0}^{n-2} P[k mod 9], where
 *            the period‑9 difference pattern is P = {5,5,5,4,5,5,5,4,4}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*---------------------------------------------------------------------*/
/*  Sequence generator:  a(idx_t n)  ---------------------------------*/
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return 1;
    if (n == 1) return 4;

    /* Number of diffs after the initial special step (n−1). */
    idx_t m = n - 1;

    /* Count full 9‑term cycles and the remaining part. */
    idx_t cycles = m / 9;
    idx_t rem    = m % 9;

    /* Prefix sums of one full period (P[0] … P[rem-1]).                */
    static const seq_t prefix[10] = {
        0, 5, 10, 15, 19, 24, 29, 34, 38, 42
    };

    /* Sum of all diffs:   cycles × period_sum + prefix[rem]            */
    seq_t sum = (seq_t)cycles * 42 + prefix[rem];

    /* Add the constant offset (1+3 = 4).                            */
    return sum + 4;
}
