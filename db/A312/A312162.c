/*
 * A312162  Coordination sequence Gal.6.233.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8. For n≥3: let k = n-2. a(n) = 8 +
 *            48*⌊k/10⌋ + s[k mod 10], where s =
 *            {0,5,10,16,22,27,32,36,40,44,48}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    idx_t k   = n - 2;               /* number of steps beyond a(2)   */
    idx_t q   = k / 10;              /* complete 10‑step blocks       */
    idx_t r   = k % 10;              /* remaining steps in the block  */

    /* Periodic prefix sums for one 10‑step block. */
    static const seq_t prefix[11] = { (seq_t)0, (seq_t)5, (seq_t)10,
                                      (seq_t)16, (seq_t)22, (seq_t)27,
                                      (seq_t)32, (seq_t)36, (seq_t)40,
                                      (seq_t)44, (seq_t)48 };

    return (seq_t)8 + ((seq_t)48 * (seq_t)q) + prefix[r];
}
