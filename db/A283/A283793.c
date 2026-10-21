/*
 * A283793  Number of elements formable in <= n steps, starting with 4
 *          elements, combining 2 elements into a new element at each step.
 * https://oeis.org/A283793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 4; a(n) = 4 + T(a(n-1)), where T(m) = m*(m+1)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = (seq_t)4;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute T(value) = value * (value + 1) / 2 */
        seq_t temp = (value * (value + (seq_t)1)) / (seq_t)2;
        /* a(i) = 4 + T(value) */
        value = temp + (seq_t)4;
    }
    return value;
}
