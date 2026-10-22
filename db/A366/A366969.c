/*
 * A366969  a(n) = Sum_{k=3..n} (k-2) * floor(n/k).
 * https://oeis.org/A366969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=3..n} (k-2) * floor(n/k)
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t sn = (seq_t)n;               /* cast n once for division */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t kval = (seq_t)k;         /* k as seq_t for arithmetic */
        seq_t q = sn / kval;           /* floor(n/k) as seq_t */
        seq_t term = (kval - (seq_t)2) * q;
        total += term;
    }
    return total;
}
