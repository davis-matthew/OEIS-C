/*
 * A282820  Number of inequivalent ways to color the edges of a tetrahedron
 *          using at most n colors so that no color appears more than twice.
 * https://oeis.org/A282820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*n*(n-1)*(n^3+3*n^2-10*n-6)/12
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3U)
        return 0;

    seq_t nseq = (seq_t)n;
    seq_t a = nseq - 2;
    seq_t b = nseq - 1;
    seq_t c = nseq;
    seq_t d = nseq * nseq * nseq + 3 * nseq * nseq - 10 * nseq - 6;

    /* Reduce factors by 12 = 2 * 2 * 3 */
    seq_t div = 12;
    seq_t *parts[4] = { &a, &b, &c, &d };

    /* We repeatedly try to divide each part by a prime factor of div
       until div becomes 1. */
    while (div != 1) {
        seq_t f = 0;
        if (div % 2 == 0)
            f = 2;
        else if (div % 3 == 0)
            f = 3;
        if (!f)
            break; /* Should not happen */

        /* Try to cancel factor f from the parts */
        int i;
        for (i = 0; i < 4 && div % f == 0; ++i) {
            if ((*parts[i]) % f == 0) {
                *parts[i] /= f;
                div /= f;
            }
        }
    }

    /* Compute the remaining product */
    seq_t prod = a;
    prod *= b;
    prod *= c;
    prod *= d;

    return prod;
}
