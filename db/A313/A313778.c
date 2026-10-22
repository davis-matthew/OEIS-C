/*
 * A313778  Coordination sequence Gal.6.203.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = 5*n + δ(n) where δ(n) = 1 if n%6==4
 *            or n%6==5 and ((n-4)/6)%2==0 -1 if n%6==1 or n%6==2 and
 *            ((n-1)/6)%2==1 0 otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t term = (seq_t)5 * (seq_t)n;
    switch (n % 6)
    {
        case 1:
            if ((((n - 1) / 6) & 1ULL)) term -= 1;
            break;
        case 2:
            if ((((n - 2) / 6) & 1ULL)) term -= 1;
            break;
        case 4:
            if ((((n - 4) / 6) & 1ULL) == 0) term += 1;
            break;
        case 5:
            if ((((n - 5) / 6) & 1ULL) == 0) term += 1;
            break;
        /* cases 0 and 3: term unchanged */
        default:
            break;
    }
    return term;
}
