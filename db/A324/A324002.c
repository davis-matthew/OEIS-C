/*
 * A324002  Row 3 of array in A324000.
 * https://oeis.org/A324002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [9,4,3,12,3,4,18,4,3,12,3,4][n mod 12]
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic values for n mod 12 */
    static const seq_t table[12] = {
        (seq_t)9,  (seq_t)4,  (seq_t)3,  (seq_t)12,
        (seq_t)3,  (seq_t)4,  (seq_t)18, (seq_t)4,
        (seq_t)3,  (seq_t)12, (seq_t)3,  (seq_t)4
    };
    return table[n % 12];
}
