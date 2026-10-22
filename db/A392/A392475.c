/*
 * A392475  Sum of the largest parts in the partitions of n into 3 parts whose
 *          middle part is even.
 * https://oeis.org/A392475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{k=j..floor((n-j)/2)}
 *            (n-j-k)*((k+1) mod 2)
 * Range:     u8 n <= 21, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t max_j = n / 3;                 /* floor(n/3) */
    for (idx_t j = 1; j <= max_j; ++j) {
        idx_t max_k = (n - j) / 2;       /* floor((n-j)/2) */
        if (max_k < j) continue;
        for (idx_t k = j; k <= max_k; ++k) {
            seq_t contrib = (seq_t)(n - j - k);
            seq_t parity = ((seq_t)(k + 1)) & 1;   /* (k+1) mod 2 */
            total += contrib * parity;
        }
    }
    return total;
}
