/*
 * A260316  n/3 if 3 divides n, else n-1.
 * https://oeis.org/A260316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/3 if n % 3 == 0, else n-1
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260316 */

/* Computes a(n) for any n >= 0 using a direct formula. */
seq_t a(idx_t n)
{
    /* If n is divisible by 3, return n/3, otherwise return n-1. */
    if ((n % 3U) == 0U)
        return (seq_t)n / (seq_t)3;
    else
        return (seq_t)n - (seq_t)1;
}
