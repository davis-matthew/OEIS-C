/*
 * A313129  Coordination sequence Gal.6.159.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313129
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
 * Formula:
 *   a(n) = 5*n + 2*floor((n-1)/12) + c[(n-1) mod 12]
 *   where c = {-1,-1,-1,-1,0,1,2,3,3,3,3,2}
 */
static const int c[12] = {-1, -1, -1, -1, 0, 1, 2, 3, 3, 3, 3, 2};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k = (n - 1) / 12;      /* floor((n-1)/12)   */
    idx_t r = (n - 1) % 12;      /* (n-1) mod 12      */

    seq_t result = (seq_t)5 * n;  /* 5*n               */
    result += (seq_t)2 * k;       /* 2*floor((n-1)/12) */

    int ck = c[r];                /* signed offset     */
    if (ck < 0)
        result -= (seq_t)(-ck);
    else
        result += (seq_t)ck;

    return result;
}
