/*
 * A304837  a(n) = 6*(n - 1)*(81*n - 104) for n >= 1.
 * https://oeis.org/A304837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(n - 1)*(81*n - 104)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Outside the defined offset (n == 0) return 0 */
    if (n == 0)
        return 0;

    seq_t n_seq   = (seq_t)n;          /* convert idx_t to seq_t */
    seq_t n_minus1 = n_seq - 1;
    seq_t part = (seq_t)81 * n_seq - (seq_t)104;
    seq_t result = 6 * (n_minus1 * part);
    return result;
}
