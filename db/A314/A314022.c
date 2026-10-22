/*
 * A314022  Coordination sequence Gal.6.203.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - f[(n-1) % 12] (n > 0), a(0) = 1 where f =
 *            {0,-1,0,0,1,0,-1,0,0,1,0,0}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* offsets: f[0] corresponds to n = 1 */
    static const int f[12] = {0, -1, 0, 0, 1, 0, -1, 0, 0, 1, 0, 0};

    idx_t idx = (n - 1) % 12;
    int  o   = f[idx];          /* signed offset */

    seq_t base = (seq_t)5 * (seq_t)n;  /* 5*n */
    if (o >= 0)
        return base - (seq_t)o;      /* subtract positive offset */
    else
        return base + (seq_t)(-o);  /* add when offset is negative */
}
