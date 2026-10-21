/*
 * A287793  Eight steps forward, seven steps back.
 * https://oeis.org/A287793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (-1)^floor((2*i-2)/15)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t m = (seq_t)(i - 1);        /* i-1, as seq_t to avoid overflow */
        seq_t k = (m * 2) / 15;          /* floor((2*(i-1))/15) */
        if ((k & 1) == 0) {
            sum += (seq_t)1;
        } else {
            sum -= (seq_t)1;
        }
    }
    return sum;
}
