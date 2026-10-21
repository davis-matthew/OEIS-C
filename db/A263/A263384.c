/*
 * A263384  Fourth column of the matrix of polynomial coefficients of the
 *          rational approximation to Mill's ratio.
 * https://oeis.org/A263384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ((2*n+6)!! - 3*(2*n+5)!! + (2*n+3)!!)/6, n>=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit intermediates for 64‑bit builds to avoid overflow. */
    unsigned __int128 E = 48;   /* 6!! */
    unsigned __int128 C = 3;    /* 3!! */
    for (idx_t i = 1; i <= n; ++i) {
        E *= (unsigned __int128)(2*i + 6);
        C *= (unsigned __int128)(2*i + 3);
    }
    unsigned __int128 B   = (unsigned __int128)(2*n + 5) * C; /* (2n+5)!! */
    unsigned __int128 num = E - 3*B + C;                     /* numerator   */
    unsigned __int128 ans = num / 6;                         /* final value */
    return (seq_t)ans;
#else
    /* Fallback using only seq_t operations – works for arbitrary‑precision seq_t. */
    seq_t E = 48;   /* 6!! */
    seq_t C = 3;    /* 3!! */
    for (idx_t i = 1; i <= n; ++i) {
        E = E * (seq_t)(2*i + 6);
        C = C * (seq_t)(2*i + 3);
    }
    seq_t B   = (seq_t)(2*n + 5) * C;   /* (2n+5)!! */
    seq_t num = E - 3*B + C;             /* numerator   */
    seq_t ans = num / 6;                 /* final value */
    return ans;
#endif
}
