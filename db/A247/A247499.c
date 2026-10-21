/*
 * A247499  a(n) = hypergeom([1, -n, -n-1], [2], 1).
 * https://oeis.org/A247499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = hypergeom([1, -n, -n-1], [2], 1); Recurrence (valid for n
 *            ≥ 4): (n-3)*(n+2)*a(n) = 2*(n³ - n² - 5*n - 1)*a(n-1) -
 *            (n-1)*(n³ - n² - 3*n - 2)*a(n-2) + (n-2)²*(n-1)²*a(n-3); Initial
 *            values: a(0)=1, a(1)=2, a(2)=6, a(3)=25
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)25;

    /* Variables to hold successive terms of the sequence. */
    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)2;   /* a(n-2)   */
    seq_t a2 = (seq_t)6;   /* a(n-1)   */
    seq_t a3 = (seq_t)25;  /* a(n)     */
    seq_t an;             /* temporary for a(n+1) */

    /* For small seq_t sizes use 128‑bit intermediates to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        for (idx_t i = 4; i <= n; ++i)
        {
            /* Compute the integer coefficients. */
            unsigned __int128 ii = i;
            unsigned __int128 c1 = ii*ii*ii - ii*ii - (unsigned __int128)5*ii - 1;
            unsigned __int128 c2 = (ii-1)*(ii*ii*ii - ii*ii - (unsigned __int128)3*ii - 2);
            unsigned __int128 c3 = (ii-2)*(ii-2)*(ii-1)*(ii-1);

            /* Compute the numerator: 2*c1*a3 - c2*a2 + c3*a1. */
            unsigned __int128 t1 = 2 * c1 * (unsigned __int128)a3;
            unsigned __int128 t2 = c2 * (unsigned __int128)a2;
            unsigned __int128 t3 = c3 * (unsigned __int128)a1;
            unsigned __int128 num = t1 + t3 - t2;

            /* Compute the denominator: (i-3)*(i+2). */
            unsigned __int128 denom = (ii-3)*(ii+2);

            an = (seq_t)(num / denom);

            /* Shift the window of last three terms. */
            a0 = a1; a1 = a2; a2 = a3; a3 = an;
        }
    }
    else {
        /* For arbitrary‑precision seq_t (e.g., big integer class), use
           seq_t arithmetic directly. */
        for (idx_t i = 4; i <= n; ++i)
        {
            seq_t c1 = (seq_t)(i*i*i - i*i - 5*i - 1);
            seq_t c2 = (seq_t)((i-1)*(i*i*i - i*i - 3*i - 2));
            seq_t c3 = (seq_t)((i-2)*(i-2)*(i-1)*(i-1));

            seq_t num = 2 * c1 * a3;
            num -= c2 * a2;
            num += c3 * a1;

            seq_t denom = (seq_t)((i-3)*(i+2));
            an = num / denom;

            a0 = a1; a1 = a2; a2 = a3; a3 = an;
        }
    }

    return a3;
}
