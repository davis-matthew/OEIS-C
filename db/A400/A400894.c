/*
 * A400894  Bipartition of the positive odd numbers depending on the parity of
 *          the 2-adic valuation A007814(3*k + 2). Here the part with even
 *          valuation based on A400891.
 * https://oeis.org/A400894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 2*A400891(n) + 1 where A400891(n) is the sequence of
 *            indices k (starting at 1) for which the 2-adic valuation of
 *            3*k+2 is even. The function below produces the nth odd number m
 *            = 2*k+1 with that property. It does so by scanning k in
 *            increasing order, counting those with even valuation, unt...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = 1;            /* current index of odd numbers (m = 2*k+1) */
    idx_t cnt = 0;          /* how many indices with even valuation we've seen */

    while (1) {
        /* Compute 3*k + 2, which fits in 64 bits for the ranges of interest. */
        unsigned long long t = 3ULL * k + 2ULL;

        /* Determine parity of v₂(t).  parity = 0 => even, 1 => odd. */
        unsigned int parity = 0;
        while ((t & 1ULL) == 0) {
            t >>= 1;
            parity ^= 1;
        }

        if (parity == 0) {          /* even 2-adic valuation */
            ++cnt;
            if (cnt == n)            /* nth element found */
                return (seq_t)(2ULL * k + 1ULL);
        }
        ++k;
    }
}
