/*
 * A332011  Let k be the least positive number such that n AND floor(n/k) = 0
 *          (where AND denotes the bitwise AND operator); a(n) = floor(n/k).
 * https://oeis.org/A332011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n/k) where k is the least positive integer such
 *            that (n AND floor(n/k)) == 0. The function returns that
 *            floor(n/k)
 * Range:     u8 n <= 292, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t m = (seq_t)n / k;            /* floor(n/k) */
        if (((seq_t)n & m) == 0) return m; /* stop at first k satisfying the condition */
    }

    /* If no k <= n works, then minimal k is n+1 and floor(n/k) == 0. */
    return 0;
}
