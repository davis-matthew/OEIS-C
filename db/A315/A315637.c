/*
 * A315637  Coordination sequence Gal.6.352.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315637
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

/* 
 * OEIS A315637 – Coordination sequence Gal.6.352.6
 *
 * Sequence definition:
 *   a(0) = 1
 *   For n ≥ 1 the sequence is given by the periodic relation
 *       a(n+10) = a(n) + 52 .
 *   The first ten values for n = 1 … 10 are
 *       6, 12, 16, 20, 26, 32, 36, 40, 46, 52 .
 *   These values fully determine the sequence for all n.
 *
 * Formula implemented below:
 *   if n == 0                     return 1;
 *   else let k = n-1,
 *        q = k / 10,
 *        r = k % 10                // r = (n-1) mod 10
 *        return base1[r] + 52 * q
 *   where base1 contains a(1) … a(10).
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* base1[r] holds a(1) .. a(10) respectively */
    static const seq_t base1[10] = {
        (seq_t)6,  (seq_t)12, (seq_t)16, (seq_t)20,
        (seq_t)26, (seq_t)32, (seq_t)36, (seq_t)40,
        (seq_t)46, (seq_t)52
    };

    idx_t k = n - 1;            /* shift so that a(1) is base1[0] */
    idx_t q = k / 10;           /* how many 10‑step periods */
    idx_t r = k % 10;           /* position within the period */

    return base1[r] + (seq_t)q * (seq_t)52;
}
