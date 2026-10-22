/*
 * A311958  Coordination sequence Gal.5.53.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + count_{k=2..n, k%10∈{3,5,6,8}} 1 This follows from
 *            the observed pattern of differences: a(0)=1, a(1)=4, and for n≥2
 *            the increment d(n)=a(n)-a(n-1) is 5 precisely when n≡3,5,6,8
 *            (mod 10), otherwise it is 4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* Main formula: a(n) = 4*n + s(n),
       where s(n) counts numbers k in [2,n] with k%10∈{3,5,6,8}. */
    seq_t result = (seq_t)4 * n;
    seq_t add = 0;
    if (n >= 3)
        add += (n - 3) / 10 + 1;
    if (n >= 5)
        add += (n - 5) / 10 + 1;
    if (n >= 6)
        add += (n - 6) / 10 + 1;
    if (n >= 8)
        add += (n - 8) / 10 + 1;
    result += add;
    return result;
}
