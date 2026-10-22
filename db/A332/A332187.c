/*
 * A332187  a(n) = 8*(10^(2n+1)-1)/9 - 10^n.
 * https://oeis.org/A332187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*(10^(2n+1)-1)/9 - 10^n Equivalent construction: a(n) is
 *            a number of length 2*n+1 digits, all 8's with a single 7 at the
 *            (n+1)-th most significant digit
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t len = 2 * n + 1;
    for (idx_t i = 0; i < len; ++i) {
        res = res * 10 + ((i == n) ? (seq_t)7 : (seq_t)8);
    }
    return res;
}
