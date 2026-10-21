/*
 * A256299  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8
 *          -> 0 to the digits of n written in base 9, then convert back to
 *          base 10.
 * https://oeis.org/A256299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ ((digit_9(n,i)+1) mod 9) * 9^i
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t result = 0;
    seq_t place  = 1;
    idx_t temp   = n;
    while (temp > 0) {
        idx_t d = temp % 9;
        seq_t e = (d < 8) ? (seq_t)(d + 1) : (seq_t)0;
        result += e * place;
        place  *= 9;
        temp   /= 9;
    }
    return result;
}
