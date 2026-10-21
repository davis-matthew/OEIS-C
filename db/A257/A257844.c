/*
 * A257844  a(n) = floor(n/4) * (n mod 4).
 * https://oeis.org/A257844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n/4) * (n mod 4)
 * Range:     u8 n <= 346, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert division and modulo operands to seq_t before multiplying
       so the calculation is performed in the target type without
       intermediate overflow in idx_t. */
    seq_t q = (seq_t)(n / 4);
    seq_t r = (seq_t)(n % 4);
    return q * r;
}
