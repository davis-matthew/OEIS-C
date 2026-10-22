/*
 * A350116  Number of ways to partition the set of vertices of a convex
 *          {n+8}-gon into 3 non-intersecting polygons.
 * https://oeis.org/A350116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n+7)*(n+8)/12
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 472, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a = (seq_t)n;
    seq_t b = (seq_t)n + 1;
    seq_t c = (seq_t)n + 7;
    seq_t d = (seq_t)n + 8;

    /* divide by 12 = 2 * 2 * 3, distributing the factors among the terms
       to keep intermediate values small and exact   */
    seq_t f[3] = {2, 2, 3};
    for (unsigned i = 0; i < 3; ++i) {
        seq_t fval = f[i];
        if ((a % fval) == 0)
            a /= fval;
        else if ((b % fval) == 0)
            b /= fval;
        else if ((c % fval) == 0)
            c /= fval;
        else
            d /= fval; /* must be divisible here */
    }

    return a * b * c * d;
}
