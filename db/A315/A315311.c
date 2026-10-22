/*
 * A315311  Coordination sequence Gal.3.51.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=10, for n≥3: a(n)=a(n-2)+10 +
 *            ((n-2)%5==4?2:0)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)10;

    seq_t a_prev_prev = (seq_t)6;   /* a(1) */
    seq_t a_prev     = (seq_t)10;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t temp = a_prev_prev + (seq_t)10;
        if (((i - 2) % (idx_t)5) == (idx_t)4)
            temp += (seq_t)2;
        a_prev_prev = a_prev;
        a_prev = temp;
    }
    return a_prev;
}
