/*
 * A352627  Number of ways to write n as a^2 + 2*b^2 + c^4 + 4*d^4 + c^2*d^2,
 *          where a,b,c,d are nonnegative integers.
 * https://oeis.org/A352627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (a,b,c,d) of non‑negative integers
 *            satisfying a^2 + 2*b^2 + c^4 + 4*d^4 + c^2*d^2 = n
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t N = n;                     /* store n as seq_t for arithmetic */

    /* a^2 part */
    for (idx_t a = 0; ; ++a) {
        seq_t a_sq = (seq_t)a * (seq_t)a;
        if (a_sq > N) break;             /* a^2 alone already too large */
        seq_t rem1 = N - a_sq;           /* remaining after a^2 */

        /* 2*b^2 part */
        for (idx_t b = 0; ; ++b) {
            seq_t b_sq = (seq_t)b * (seq_t)b;
            seq_t two_b_sq = (seq_t)2 * b_sq;
            if (two_b_sq > rem1) break;  /* 2*b^2 too large */
            seq_t rem2 = rem1 - two_b_sq;/* remaining after a^2 + 2*b^2 */

            /* c^4 part */
            for (idx_t c = 0; ; ++c) {
                seq_t c_sq = (seq_t)c * (seq_t)c;
                seq_t c4   = c_sq * c_sq;      /* c^4 */
                if (c4 > rem2) break;           /* c^4 too large */
                /* remaining after a^2 + 2*b^2 + c^4 */
                /* need c^2*d^2 + 4*d^4 = rem2 - c4 */

                for (idx_t d = 0; ; ++d) {
                    seq_t d_sq = (seq_t)d * (seq_t)d;
                    seq_t d4   = d_sq * d_sq;   /* d^4 */
                    seq_t val = c4 + (seq_t)4 * d4 + c_sq * d_sq;
                    if (val > rem2) break;          /* too big, next d would be larger */
                    if (val == rem2) {
                        ++count;               /* found a valid (a,b,c,d) */
                        break;                 /* larger d will only increase val */
                    }
                }
            }
        }
    }

    return count;
}
