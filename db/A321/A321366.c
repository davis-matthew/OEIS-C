/*
 * A321366  a(n) is the least integer k greater than 1 such that n divides
 *          binomial(k, 2) = A000217(k-1).
 * https://oeis.org/A321366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the least integer k > 1 such that n | binomial(k, 2)
 * Range:     u8 n <= 24, u16 n <= 1020, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function scans k from 2 up to 2*n (which always works), testing
 * the divisibility condition.  For n ≤ 10000 this loop executes at most
 * 20000 iterations, well within the time limit.  The arithmetic is
 * performed in seq_t to match the build’s integer type. */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;

    /* Special case for n == 1, though the loop handles it naturally. */
    if (nn == (seq_t)0) return (seq_t)0;   /* sentinel, should not happen */

    for (seq_t k = 2; ; ++k) {
        /* Compute binomial(k,2) = k*(k-1)/2 safely. */
        seq_t t = k * (k - (seq_t)1) / (seq_t)2;
        /* Check divisibility by n. */
        if ((t % nn) == (seq_t)0)
            return k;
    }
}
