/*
 * A374008  a(n) = (1 + (n+6)^2 + (n-7)*(-1)^n)/2.
 * https://oeis.org/A374008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+6)^2 + (n-7)*(-1)^n)/2
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    __int128 tn = n;
    __int128 val = (__int128)1 + (tn + 6) * (tn + 6);
    if ((n & 1) == 0) {
        val += (tn - 7);
    } else {
        val -= (tn - 7);
    }
    val /= 2;
    return (seq_t)val;
}
