/*
 * A375489  Omit the trailing zeros from double factorial of even numbers
 *          (A000165).
 * https://oeis.org/A375489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (2*n)!! with all trailing zeros removed
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t k = 1; k <= n; ++k) {
        val *= (seq_t)(2 * k);
        while ((val % 10) == 0) {
            val /= 10;
        }
    }
    return val;
}
