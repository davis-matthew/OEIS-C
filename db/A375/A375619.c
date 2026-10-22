/*
 * A375619  a(n) is the largest integer such that there exists a simple graph
 *          with n vertices, a(n) edges, and no cycles of length 0 mod 4.
 * https://oeis.org/A375619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((19/12)*(n-1))
 * Range:     u8 n <= 14, u16 n <= 4543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  If n==0 we return 0
       for safety, although the offset starts at 1. */
    if (n == 0)
        return 0;

    idx_t nm1 = n - 1;          /* n-1 as idx_t */
    seq_t prod = (seq_t)19 * (seq_t)nm1; /* 19*(n-1) in seq_t */
    return prod / 12;           /* integer division = floor for positive */
}
