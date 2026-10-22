/*
 * A361608  a(n) = 7^n*(n+1)*(81*n^4+684*n^3+1401*n^2+434*n+40)/40.
 * https://oeis.org/A361608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n*(n+1)*(81*n^4+684*n^3+1401*n^2+434*n+40)/40
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast exponentiation: compute 7^n */
    seq_t pow7 = 1;
    seq_t base = 7;
    idx_t e = n;
    while (e) {
        if (e & 1) {
            pow7 *= base;
        }
        base *= base;
        e >>= 1;
    }

    /* compute polynomial part: 84*n^4 + ... + 40 */
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;
    seq_t n3   = n2 * nseq;
    seq_t n4   = n3 * nseq;
    seq_t poly = 81 * n4 + 684 * n3 + 1401 * n2 + 434 * nseq + 40;

    /* divide by 40 first: ((n+1)*poly)/40  is integer */
    seq_t t = (nseq + 1) * poly;
    t = t / 40;

    return pow7 * t;
}
