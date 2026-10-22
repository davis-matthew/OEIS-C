/*
 * A315348  Coordination sequence Gal.3.59.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0: if n is odd: let m = (n-1)/2 a(n) = 6*n - 2
 *            * (m & 1) else (n even): let m = n/2 if m is even: a(n) = 11*n /
 *            2 else if m == 1: a(n) = 6*n - 2 else // m odd and >1 a(n) =
 *            10*m + 2 This piecewise definition reproduces the first 24 terms
 *            of OEIS sequence A315348
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* handle odd indices */
    if (n & 1) {
        idx_t m = (n - 1) >> 1;          /* (n-1)/2  */
        seq_t res = ((seq_t)6 * n);      /* 6*n      */
        if (m & 1)                       /* subtract 2 when m is odd */
            res -= 2;
        return res;
    }

    /* handle even indices (n > 0) */
    idx_t m = n >> 1;                    /* n/2      */
    if ((m & 1) == 0) {                  /* m even   */
        seq_t res = ((seq_t)11 * n);     /* 11*n     */
        res /= 2;                        /* integer division */
        return res;
    }

    /* m is odd */
    if (m == 1) {                        /* n = 2    */
        seq_t res = ((seq_t)6 * n);     /* 6*n - 2  */
        res -= 2;
        return res;
    }

    /* m odd and > 1 */                  /* n = 6,10,14,... */
    seq_t res = ((seq_t)10 * m);         /* 10*m     */
    res += 2;
    return res;
}
