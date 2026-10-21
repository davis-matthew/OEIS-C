/*
 * A253418  Number of (n+2)X(2+2) nonnegative integer arrays with all values
 *          the knight distance from the upper left minus as much as 1, with
 *          successive minimum path knight move differences either 0 or +1,
 *          and any unreachable value zero.
 * https://oeis.org/A253418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 56, a(2) = 131, a(3) = 1087, a(4) = 2827, a(5) = 10411,
 *            a(6) = 15803 For n > 6 the sequence satisfies the closed‑form
 *            polynomial: - If n is even: a(n) = 12*n^4 + (46/3)*n^3 +
 *            (577/4)*n^2 - (17195/6)*n + 8989 - If n is odd : a(n) = 12*n^4 +
 *            (190/3)*n^3 + (41/4)*n^2 - (7123/3)*n + 26887/4 (T...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 73, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 6 terms supplied directly */
    static const seq_t base[6] = {56, 131, 1087, 2827, 10411, 15803};

    if (n <= 6) return base[n - 1];

    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;          /* n^2   */
    seq_t n3   = n2   * nseq;          /* n^3   */
    seq_t n4   = n2   * n2;            /* n^4   */

    /* use LCM 12 for all denominators */
    if ((n & 1) == 0) { /* n even */
        seq_t num = 12 * 12 * n4            /* 12*n^4 */
                 + 46 * 4 * n3              /* (46/3)*n^3 */
                 + 577 * 3 * n2            /* (577/4)*n^2 */
                 - 17195 * 2 * nseq        /* -(17195/6)*n */
                 + 8989 * 12;              /* 8989 */
        return num / 12;
    } else { /* n odd */
        seq_t num = 12 * 12 * n4            /* 12*n^4 */
                 + 190 * 4 * n3             /* (190/3)*n^3 */
                 + 41 * 3 * n2             /* (41/4)*n^2 */
                 - 7123 * 4 * nseq         /* -(7123/3)*n */
                 + 26887 * 3;              /* 26887/4 */
        return num / 12;
    }
}
