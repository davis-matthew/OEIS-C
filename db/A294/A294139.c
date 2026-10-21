/*
 * A294139  Sum of the areas of the distinct rectangles (and the areas of the
 *          squares on their sides) with positive integer sides such that L +
 *          W = n, W < L.
 * https://oeis.org/A294139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1}^{⌊(n-1)/2⌋} (2*i^2 + 2*(n-i)^2 + i*(n-i))
 *            Simplified: a(n) = n*(6*n-1)*(n-2)/8 when n is even a(n) =
 *            n*(3*n-1)*(n-1)/4 when n is odd Only integer operations are
 *            used; intermediate values are kept in a 64‑bit unsigned integer
 *            which is sufficient for n up to at least 10,000 (the re...
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The input type idx_t is an unsigned integer (uint64_t). */
    unsigned long long nn = n;          /* use 64‑bit for intermediates */

    if ((n & 1) == 0) {  /* even n */
        unsigned long long res =
            nn * ((unsigned long long)6 * nn - 1ULL) *
            (nn - 2ULL) / 8ULL;
        return (seq_t)res;
    } else {            /* odd n */
        unsigned long long res =
            nn * ((unsigned long long)3 * nn - 1ULL) *
            (nn - 1ULL) / 4ULL;
        return (seq_t)res;
    }
}
