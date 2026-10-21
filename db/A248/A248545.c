/*
 * A248545  a(n+1) = a(n)^2 + 75.
 * https://oeis.org/A248545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(n+1) = a(n)^2 + 75 */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        result = result * result + (seq_t)75;
    }
    return result;
}
