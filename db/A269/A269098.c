/*
 * A269098  Expansion of (1 + 2*x + 3*x^2 + x^3 + x^5)/(1 - x^3)^2.
 * https://oeis.org/A269098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/3)+1 if n mod 3 = 0 a(n) = 2*floor(n/3)+2 if n
 *            mod 3 = 1 a(n) = 4*floor(n/3)+3 if n mod 3 = 2
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    idx_t r = n % 3;
    seq_t res;

    if (r == 0) {
        res = (seq_t)2 * (seq_t)q + (seq_t)1;
    } else if (r == 1) {
        res = (seq_t)2 * (seq_t)q + (seq_t)2;
    } else { /* r == 2 */
        res = (seq_t)4 * (seq_t)q + (seq_t)3;
    }

    return res;
}
