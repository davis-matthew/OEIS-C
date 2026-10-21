/*
 * A267142  The characteristic function of the multiples of 9.
 * https://oeis.org/A267142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n % 9 == 0 else 0
 * Range:     u8 n <= 269, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 9 in seq_t type */
    seq_t m = (seq_t)n % (seq_t)9;
    return m == 0 ? (seq_t)1 : (seq_t)0;
}
