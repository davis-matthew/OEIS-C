/*
 * A315335  Coordination sequence Gal.6.624.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-12) + 66 for n >= 13, with base values a(0)=1,
 *            a(1)=6, a(2)=10, a(3)=16, a(4)=22, a(5)=27, a(6)=33, a(7)=39,
 *            a(8)=44, a(9)=50, a(10)=56, a(11)=60, a(12)=66
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[13] = {
        (seq_t)1,  /* a(0) */
        (seq_t)6,  /* a(1) */
        (seq_t)10, /* a(2) */
        (seq_t)16, /* a(3) */
        (seq_t)22, /* a(4) */
        (seq_t)27, /* a(5) */
        (seq_t)33, /* a(6) */
        (seq_t)39, /* a(7) */
        (seq_t)44, /* a(8) */
        (seq_t)50, /* a(9) */
        (seq_t)56, /* a(10) */
        (seq_t)60, /* a(11) */
        (seq_t)66  /* a(12) */
    };

    if (n == 0) return base[0];

    idx_t m = (n - 1) / 12;          /* number of 12‑step jumps */
    idx_t r = n - 12 * m;            /* remaining index (1 ≤ r ≤ 12) */

    return base[r] + (seq_t)66 * (seq_t)m;
}
