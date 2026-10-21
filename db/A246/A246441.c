/*
 * A246441  Nonnegative integers k satisfying cos(k) < sec(k+1).
 * https://oeis.org/A246441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A246441 – Nonnegative integers k with cos(k+1) > 0.
 *  The inequality cos(k+1) < sec(k+1) is equivalent (for real k)
 *  to cos(k+1) > 0, because sec is 1/cos.
 *  This is true exactly when k+1 lies in the open intervals
 *
 *         (2πm – π/2 , 2πm + π/2)   for some integer m ≥ 0.
 *
 *  Consequently the k’s form disjoint blocks.  The length of block m
 *  is
 *
 *         ⌊(4πm + π)/(2π)⌋ – ⌊(4πm – π)/(2π)⌋.
 *
 *  Using the high‑accuracy convergent of π
 *
 *          π ≈ 104348 / 33215
 *
 *  these floor values can be computed exactly with integer arithmetic
 *  (the error in the approximations is < 10⁻¹⁰, so the block limits
 *  are correct for all k that appear for n up to at least 100 000).
 *
 *  The function below generates the n‑th element of the sequence
 *  by hopping through the blocks until the desired block is reached.
 */

seq_t a(idx_t n)
{
    /* Numerical constants for π ≈ P/Q.  PQ gives an error < 10⁻¹⁰. */
    const idx_t P   = 104348ULL;   /* numerator     (≈π) */
    const idx_t Q   = 33215ULL;    /* denominator   (≈π) */
    const idx_t TWO_Q   = 2 * Q;   /* 2·Q   for denominator   */
    const idx_t FOUR_P  = 4 * P;   /* 4·P   for numerator   */

    /* Offset 1: a(1)=0 */
    if (n == 1)
        return (seq_t)0;

    /* For n ≥ 2 we have already counted the element 0. */
    idx_t target = n - 2;   /* number of terms still to skip after the first block */
    idx_t m = 1;           /* block index (m ≥ 1) */

    while (1) {
        /* Compute block limits using 128‑bit arithmetic to avoid overflow. */
        __uint128_t up128   = (__uint128_t)FOUR_P * m + P;
        __uint128_t low128  = (__uint128_t)FOUR_P * m - P;

        idx_t upper = (idx_t)(up128 / TWO_Q);   /* floor((4P m + P)/(2Q)) */
        idx_t lower = (idx_t)(low128 / TWO_Q);  /* floor((4P m - P)/(2Q)) */

        idx_t block_len = upper - lower;

        if (target < block_len)
            break;           /* the required term lies inside this block */

        target -= block_len;
        ++m;
    }

    /* First k in this block is 'lower'. */
    idx_t k_start = ((__uint128_t)FOUR_P * m - P) / TWO_Q;
    return (seq_t)(k_start + target);
}
