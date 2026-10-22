/*
 * A312143  Coordination sequence Gal.5.55.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 4*n + f(n), where f(n) = 6*(n/10) +
 *            0 if n%10 in {0,1,2}, 6*(n/10) + (n%10-2) if n%10 in
 *            {3,4,5,6,7,8}, 6*(n/10) + 6 if n%10 = 9
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 0 */
        return (seq_t)1;

    idx_t cycles = n / 10;     /* number of full 10‑blocks */
    idx_t rem    = n % 10;     /* remainder */

    seq_t f = (seq_t)6 * cycles;

    if (rem >= 3 && rem <= 8)
        f += (seq_t)(rem - 2);
    else if (rem == 9)
        f += (seq_t)6;

    return (seq_t)4 * (seq_t)n + f;
}
