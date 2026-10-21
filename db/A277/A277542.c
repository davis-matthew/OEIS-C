/*
 * A277542  a(n) = denominator((n^2 + 3*n + 2)/n^3).
 * https://oeis.org/A277542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = n^3 / 2^min(v2(n+2), 3*v2(n))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* v2(n) */
    idx_t v2n = 0;
    idx_t t = n;
    while ((t & 1) == 0) {
        t >>= 1;
        ++v2n;
    }

    /* v2(n+2) */
    idx_t v2nplus2 = 0;
    t = n + 2;
    while ((t & 1) == 0) {
        t >>= 1;
        ++v2nplus2;
    }

    /* k = min(v2(n+2), 3*v2(n)) */
    idx_t k = v2nplus2;
    idx_t thrice = 3 * v2n;
    if (thrice < k) k = thrice;

    /* compute n^3 */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;

    /* compute 2^k */
    seq_t div = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        div *= 2;
    }

    return N3 / div;
}
