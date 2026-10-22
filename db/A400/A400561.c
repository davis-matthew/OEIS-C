/*
 * A400561  a(n) is the maximum numbers of regions (both closed or open)
 *          outside a strictly convex n-polygon, delimited by the n edges and
 *          the n*(n-3)/2 diagonals of the polygon itself, where all these
 *          elements (edges and diagonals) are extended to infinity as
 *          straight lines.
 * https://oeis.org/A400561
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 2*binomial(n,2) + 2*binomial(n,4) a(n) = n*(n-1) +
 *            n*(n-1)*(n-2)*(n-3)/12 for n >= 2
 * Range:     u8 n <= 8, u16 n <= 31, u32 n <= 477, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* 2*binomial(n,2) = n*(n-1) */
    seq_t result = (seq_t)n * (seq_t)(n - 1);

    /* 2*binomial(n,4) (only for n >= 4) */
    if (n >= 4) {
        /* Avoid overflow in intermediate calculations by cancelling the
           denominator 24 = 2*2*2*3 before multiplying the numerators. */
        seq_t p[4] = { (seq_t)n,
                       (seq_t)(n - 1),
                       (seq_t)(n - 2),
                       (seq_t)(n - 3) };

        seq_t denom[4] = { (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)3 };

        for (idx_t d = 0; d < 4; ++d) {
            seq_t div = denom[d];
            for (idx_t i = 0; i < 4; ++i) {
                if (p[i] % div == 0) {
                    p[i] /= div;
                    break;
                }
            }
        }

        seq_t bin4 = p[0];
        bin4 *= p[1];
        bin4 *= p[2];
        bin4 *= p[3];

        result += bin4 * 2;   /* 2*binomial(n,4) */
    }

    return result;
}
