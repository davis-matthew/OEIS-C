/*
 * A329194  a(n) = floor(log_3(n^2)) = floor(2 log_3(n)).
 * https://oeis.org/A329194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_3(n^2)) = floor(2*log_3(n))
 * Range:     u8 n <= 15, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n² as a seq_t to avoid overflow of idx_t. */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Find largest k such that 3^k ≤ n². */
    idx_t k = 0;
    seq_t pow = 1;              /* pow = 3^k */

    /* Avoid potential overflow by checking pow <= n_sq / 3 before multiplication. */
    while (pow <= n_sq / 3) {
        pow *= 3;
        k++;
    }

    return (seq_t)k;
}
