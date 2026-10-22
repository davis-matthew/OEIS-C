/*
 * A301941  a(n) is the smallest positive integer k such that n + k divides
 *          n^2 + k, or 0 if no such k exists.
 * https://oeis.org/A301941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = minimal k > 0 such that n + k | n^2 + k (return 0 if no
 *            such k). Implementation uses the divisor characterization: n + k
 *            divides n^2 + k ⇔ n + k divides n(n−1) Thus a(n) is the smallest
 *            d > n dividing n(n−1) minus n
 * Range:     u8 n <= 16, u16 n <= 782, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for n = 0,1,2 as per the sequence definition. */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 0;

    /* Compute m = n*(n−1) in the wide type seq_t to avoid overflow. */
    seq_t m = (seq_t)n * (seq_t)(n - 1);

    /* Search for the largest divisor d ≤ n−1 of m that yields
       a complementary divisor f = m/d > n.  The first such divisor
       encountered while descending guarantees the minimal k = f−n. */
    for (idx_t d = n - 1; d >= 1; --d)
    {
        if (m % (seq_t)d == 0)
        {
            seq_t f = m / (seq_t)d;      /* complementary divisor */
            if (f > (seq_t)n)            /* must be larger than n      */
                return f - (seq_t)n;     /* minimal k found              */
        }
        /* Avoid unsigned underflow when d reaches 0 (loop terminates naturally). */
    }
    return 0;   /* No divisor > n exists (happens only for n = 2). */
}
