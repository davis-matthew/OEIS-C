/*
 * A324266  a(n) = 2*49^n.
 * https://oeis.org/A324266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*49^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponentiation by squaring: compute 49^n into result,
       then multiply by 2. */
    seq_t result = 1;
    seq_t base   = 49;
    idx_t e = n;
    while (e > 0) {
        if (e & 1) {
            result *= base;
        }
        e >>= 1;
        if (e == 0) break;
        base *= base;
    }
    return result * 2;
}
