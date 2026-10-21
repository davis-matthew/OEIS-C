/*
 * A284517  Periodic with period [1, 4, 3, 4, 1, 6] of length 6.
 * https://oeis.org/A284517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic with period [1, 4, 3, 4, 1, 6]
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = {
        (seq_t)1, (seq_t)4, (seq_t)3, (seq_t)4, (seq_t)1, (seq_t)6
    };
    return period[(n - 1) % 6];
}
