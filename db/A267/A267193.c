/*
 * A267193  Complement obverse of n.
 * https://oeis.org/A267193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse( (9-d) for each decimal digit d of n ), leading
 *            zeros omitted
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)9;
    seq_t res = 0;
    while (n > 0) {
        idx_t d = n % 10;
        seq_t c = (seq_t)(9 - d);
        res = res * 10 + c;
        n = n / 10;
    }
    return res;
}
