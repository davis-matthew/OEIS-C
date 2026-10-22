/*
 * A339601  Starting from x_0 = n, iterate by dividing with 3 (discarding any
 *          remainder), until zero is reached: x_1 = floor(x_0/3), x_2 =
 *          floor(x_1/3), etc. Then a(n) = Sum_{i=0..} (x_i AND 2^i), where
 *          AND is bitwise-and.
 * https://oeis.org/A339601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i>=0} (floor(n/3^i) & 2^i)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t res = 0;
    idx_t i = 0;
    for (;;) {
        if (x & ((seq_t)1 << i))
            res += ((seq_t)1 << i);
        if (x == 0)
            break;
        x /= 3;
        i++;
    }
    return res;
}
