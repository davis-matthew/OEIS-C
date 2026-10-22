/*
 * A340448  Radio number of the cycle graph C_n.
 * https://oeis.org/A340448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)/2*floor(n/4)+n-1 for n even; a(n) = (n+3)(n-1)/8
 *            for n ≡ 1 (mod 4); a(n) = (n-1)/2*(floor(n/4)+2) for n ≡ 3 (mod
 *            4)
 * Range:     u8 n <= 43, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* not used for the sequence offset */

    if (n & 1ULL)                             /* odd n */
    {
        if (n % 4ULL == 1ULL)
        {
            seq_t nn = (seq_t)n;
            return ((nn + (seq_t)3) * (nn - (seq_t)1)) / (seq_t)8;
        }
        else /* n % 4 == 3 */
        {
            seq_t nn = (seq_t)n;
            seq_t floor_n4 = nn / (seq_t)4;           /* floor(n/4) */
            seq_t term1    = (nn - (seq_t)1) / (seq_t)2; /* (n-1)/2 */
            seq_t term2    = floor_n4 + (seq_t)2;        /* floor(n/4)+2 */
            return term1 * term2;
        }
    }
    else                                       /* even n */
    {
        seq_t nn = (seq_t)n;
        seq_t half_n_minus_2 = (nn - (seq_t)2) / (seq_t)2; /* (n-2)/2 */
        seq_t floor_n4       = nn / (seq_t)4;              /* floor(n/4) */
        seq_t prod           = half_n_minus_2 * floor_n4;
        return prod + nn - (seq_t)1;
    }
}
