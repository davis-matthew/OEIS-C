/*
 * A312891  Coordination sequence Gal.6.216.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + f(n), where * f(n) = 1 + 2*k for n >= 2 and (n-2)
 *            mod 11 < 8, * f(n) = 2 + 2*k for n >= 2 and (n-2) mod 11 >= 8, *
 *            k = floor((n-2)/11). * Special cases: a(0) = 1, a(1) = 4
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
    if (n == 1)
        return (seq_t)4;

    /* n >= 2 */
    idx_t m = n - 2;                /* offset from n = 2 */
    idx_t cycle = m / 11;           /* how many whole 11‑index cycles before n */
    idx_t rem   = m % 11;           /* position inside the current cycle        */

    seq_t e;
    if (rem < 8)                    /* first 8 indices of a cycle */
        e = (seq_t)(1 + 2 * cycle);
    else                            /* last 3 indices of a cycle */
        e = (seq_t)(2 + 2 * cycle);

    return ((seq_t)4 * n) + e;      /* final formula */
}
